# Links

[PowerPC Floating-Point Architecture(IM:PN)](http://mirror.informatimago.com/next/developer.apple.com/documentation/mac/PPCNumerics/PPCNumerics-146.html)

# Notes

bso: branch on summary overflow (sticky bit that gets set when any of a number of overflow / exception bits get set?)

I think this may not have to do with floats specifically and just the CR?

# FPSCR: floating-point status and control register

> The floating-point status and control register (FPSCR) contains the floating-point exception
signal bits, exception summary bits, exception enable bits, and rounding control bits needed
for compliance with the IEEE-754 standard.

Bits 15-19 of the FPSCR control whether the comparison operation was less-than, greater-than, equal, or "unordered" (when comparing with NaN?)

# Normal floating point instructions:

fadd: add two doubles
fadds: add two floats
fmadd: multiply-add doubles
fmadds: multiply-add singles
fmr frD, frB: move register
fneg frD, frb: negate
  Note that there seems to be no fnegs, so I think this works both on floats and doubles?
frsp frD, frB: round double to single 

mcrfs: move some data from a FPSCR field to a CR field (each field is 4 bits?)

# Gekko

`HID2[PSE] = 1` means "processor is in paired-single mode"
Normal powerpc single-precision float instructions operate on PS0 and duplicate the result in PS0 and PS1 of the destination register

ps_add: add paired singles
ps_mr: paired single move

I think the float registers are laid out like this for paired-singles mode:
PS1|PS0

So that when you `lfs` (load floating single) a float into a register, it's automatically in PS0

## ps_merge00 frD, frA, frB: merge high

frD(ps0) ← frA(ps0)
frD(ps1) ← frB(ps0)

## ps_merge01 frD, frA, frB: merge direct

frD(ps0) ← frA(ps0)
frD(ps1) ← frB(ps1)

## ps_merge10 frD, frA, frB: merge swap

frD(ps0) ← frA(ps1)
frD(ps1) ← frB(ps0)

## ps_merge11 frD, frA, frB: merge low

frD(ps0) ← frA(ps1)
frD(ps1) ← frB(ps1)

## psq_l frD, d(rA), W, I

If W == 0:
  load frD PS0 with the first float at address d(rA)
  load frD PS1 with the second float at address d(rA)
Else:
  load frD PS0 with the first float at address d(rA)
  load frD PS1 with 1.0f

The way PS0 and PS1 are loaded may differ depending on I...
  (which is a GQR control register number)

# Discussion on gekko floating-point representation

yeah but "holds a double" is a PS thing
When in paired single mode (HID2[PSE] = 1), all the double-precision instructions are still valid,and  execute  as  in  non-paired  single  mode.  In  paired  single  mode,  all  the  single-precisionfloating-point instructions (fadds,fsubs,fmuls,fdivs,fmadds,fmsubs,fnmadds,fnmsubs, fres,frsp) are valid, and operate on the ps0 operand (the double-precision operand, in the case of frsp) of the specified registers

EntranceJewToday at 8:20 PM
a person that defines himself on liking a construct web-export port of what used to be a flash game https://levelpalace.fandom.com/wiki/Super_Mario_Construct can't run noclip? i can only imagine what device this person is using.

ComplexPlaneToday at 8:58 PM
hm I'm still very confused
supposedly single-precision floats are converted to double-precision when loaded into a float register, and converted back to single when stored (with lfs / stfs)
does that mean that fadds and friends operate on doubles that are "acting like single-precision" or are they only operating on say the lower 32-bits of the register?
aghhh
in paired-single mode it seems clear you're operating on 32-bit halves of the register at a time that naturally represent 32-bit floats

ComplexPlaneToday at 9:09 PM
> Every instruction that interprets the contents of an FPR as a floating-point value uses the double-precision floating-point format for this interpretation. ... For single-precision arithmetic and store instructions, all input values must be representable in single-precision format; otherwise, the result placed into the target FPR (or the memory location) and the setting of status bits in the FPSCR and in the condition register (if the instruction’s record bit, Rc, is set) are undefined.
so "representable in single-precision format" seems to be key here... I'm not sure what that means

beholdnecToday at 9:22 PM
In classic PowerPC, floating point registers always hold double-precision floating point values. To load a single-precision value, the instruction lfs converts values to double and stores them in the floating-point register.
Gekko introduces the paired-single format. It's tempting to think of a paired-single as if it was two 32-bit floats placed side-by-side in a single 64-bit register, but this is wrong.
You see, a floating-point register isn't literally 64-bits of storage inside the CPU. Instead, it is common for floats to be expanded to an internal representation made of exponent, mantissa, flag bits and other goodies. The details of this representation depend on the architecture, but all of the internal structures combine to represent a floating-point register.
So when IBM designed Gekko, they extended the floating-point structures to accomodate a second float in addition to the first.
For the purposes of reverse engineering, it's helpful to think of a paired-single as a 32-bit float (the ps1 component) that exists alongside the original 64-bit floating-point register from classic PowerPC, (which is now the ps0 component).

ComplexPlaneToday at 9:34 PM
Thank you, this is what I was looking for
So what exactly am I seeing when I look at the 64-bit contents of a fp register in Dolphin then?

beholdnecToday at 9:35 PM
I'm not sure how Dolphin works, but if I had to guess, it is showing the ps0 component in double representation.
I learned all this stuff while I was fixing issues with the Ghidra Gekko extensions. See: https://github.com/beholdnec/ghidra-gekko-broadway-lang/tree/pairedsingle-fixes
GitHub
beholdnec/ghidra-gekko-broadway-lang
Ghidra language definition for the Gekko and Broadway CPU variant used in the Nintendo GameCube and Nintendo Wii respectively - beholdnec/ghidra-gekko-broadway-lang

Jasper ⭐Today at 9:39 PM
Note that Dolphin still has PS accuracy issues
ComplexPlaneToday at 9:40 PM
Nice, and this was never merged into aldelaro's repo? Looks worthwhile to use this fork I suppose then

Jasper ⭐Today at 9:40 PM
aldelaro is taking a break from GC hacking

ComplexPlaneToday at 10:02 PM
So that brings me back to, what does "representable as single-precision" mean? Does it just mean essentially "originally loaded using lfs or rounded using frsp, and then having only had single-precision floating-point operations applied to it with fadds and friends"? Should I assume the internal representation will always work when using fadd and friends (even if rounded with frsp or loaded with lfs), but the register content must have been "rounded" to single-precision at some point before using fadds and such? If a function begins with frsp ?, f1 is it reasonable to assume that function takes a double instead of a float?

beholdnecToday at 10:44 PM
I'm honestly not sure. I'm guessing that when your document says "representable as single-precision", it means the value must be within the range of 32-bit floats, i.e. (-3.4028234664 x 10^38, 3.4028234664 x 10^38).
Otherwise it could break in unexpected ways.

ComplexPlaneToday at 10:53 PM
That sounds reasonable
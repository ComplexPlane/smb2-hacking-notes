# Bounty

This is an attempt to find what could be the most interesting debug menu to exist. There's quite a bit of text only in the gamecube version that hints of its existence. In order to motivate, a bounty has been set up of $20 to whomever finds the way to enable the menu first. Extra $10 if this is found on PAL as well.
- $20 bounty for NTSC
- $10 bounty for PAL
Payouts will be done through paypal, just send a link and label which version. If paypal does not work, dm Gonquai or Jcool114 for alternative methods.
Debug menu will be defined as, a controllable table which can actively modify values in game with only the controller and a cheat code. The functionality of said table will only be covered if it contains 50% or more of the possible debug commands listed on https://tcrf.net/Super_Monkey_Ball_Adventure (under Development Text).

Why the gamecube version? It's the most played version after all, and only which has the debug menu extensively suggested in the files. Even the crash handler was disabled on all other versions of the game, just makes it seem even more likely that of any version this could exist on, this is it.

If not found by 1/1/2020 MST, then this bounty is immediately nullified.

# Links

[Super Monkey Ball Adventure - The Cutting Room Floor](https://tcrf.net/Super_Monkey_Ball_Adventure)

[msysgit/msys: Git mirror of git://mingw.git.sourceforge.net/gitroot/mingw/msys-runtime (some old CVS branches are still available)](https://github.com/msysgit/msys)

Appears to be compiled with this stdlib? Found matches in some string functions
More specifically, appears to be built with newlib, which is the C stdlib implementation bundled with cygwin (which is required to use the official gamecube sdk)

# Stuff

0x803b8b10

0x4e800021: blrl opcode
0x801bc6a0: address of a blrl from jump table
  If you nop it, hud widgets don't render, neither does the pause menu, and the game doesn't advance state after clearing a stage (monkey target)

80603280: address of pointer to region in mystery table above debug function pointer
8038d920: potentially base address of region of mystery table with debug function pointer

804e650c: whether game is in "debug mode" or not?
  0 seems to mean we're in a debug mode?
  -1 might mean we're not, at least I observed that value once

80329334: first instruction to write -1 to above debug mode indicator?

0x805fd400: default start of arena
0x817d60e0: default end of arena
0x805fd400: top of stack
  Default arena also starts here. Stuff can be written near this address by functions (LR save?), so should inject stuff away from here

809629a8: whether widescreen is enabled (4 bytes)
80962a00: whether rumble is enabled
80962a58: whether camera tilt mode is enabled

# Ideas

<!-- Look for instruction which loads debug menu function pointer into r0, based on offset in table -->
<!-- Look for debug menu function pointer in RAM because why not -->
Look for code which depends on button presses
Look for code which modifies variables seemingly controlled by debug menu text options
See what effect on RAM is of debug text root function
<!-- See what happens when non-existent button bit fields are set -->
  <!-- Answer: no debug menu, but other control stick inputs cease -->

# Input

8050bc60: start of controller inputs
  Seems copied from 8050bc60, never read from?
  "previous" controller inputs?
8050bc90: mirrored controller inputs
  Seems to be the actual inputs read from by the game, read from 80554078?
  Written by parent of 803626c8 (and sibling of 803626c8)
80554078: another controller inputs location (2 words)
  Seems to be read once per frame
803626c8: function that reads both controller input words at 80554078 into a struct whose pointer was passed as an argument

## Input format for 80554078

### Word 0

Bits 0-2: unknown
Bit 3: Start
Bit 4: Y
Bit 5: X
Bit 6: B
Bit 7: A
Bit 8: unknown
Bit 9: Left trigger button
Bit 10: Right trigger button
Bit 11: Z
Bit 12: Dpad up
Bit 13: Dpad down
Bit 14: Dpad right
Bit 15: Dpad left
Bits 16-23: Left stick X axis (right is higher)
Bits 24-31: Left stick Y axis (up is higher)

### Word 1

Bits 0-7: c-stick X axis (right is higher)
Bits 8-15: c-stick Y axis (up is higher)
Bits 16-23: Left analog trigger (pressed in is higher)
Bits 24-31: Right analog trigger (pressed in is higher)

# Tracing lots_of_debug_text_something

Line 10: args = 806ab2a0, 803937e0, 8048fb24
                ^^^^ passed to root debug text function as first arg
Line 15: args = 809f8a68, 80392f58, 0000001a, 806b9750, 00000001
Line 25: args = 809f5a90, 803937f0, 00000000, 80962d90, 00000001
Line 31: args = 809f5af0, 80393800, 00000000, 80962df0, 00000001
Line 36: args = 809f5b20, 80393810, 00000000, 80960cb8, 00000001

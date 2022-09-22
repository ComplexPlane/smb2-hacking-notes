powerpc_basics.pdf is good starting point / refresher

# stwu

Store Word with Update
stwu r1, -0x20(r1) # stores r1 at address r1 - 0x20 and does r1 = r1 - 0x20
This is to set up the linkage area for the current stack frame that the callee can use to return
This is done atomically so the stack frame of the function is always in a valid state

# Stack

| Reg             | Description                                      |
| --------------- | ------------------------------------------------ |
| R0              | language-specific volatile                       |
| R1              | Stack pointer                                    |
| R2              | Read-only small data area anchor                 |
| R3-R10          | Volatile / parameter passing                     |
| R3-R4           | generally for parameter passing or return values |
| R11-R12         | volatile                                         |
| R13             | read-write small data anchor                     |
| R14-R31         | non-volatile                                     |
| F0              | volatile (language-specific)                     |
| F1              | volatile, parameter passing / return values      |
| F2-F8           | volatile (parameter passing)                     |
| F9-F13          | volatile                                         |
| F14-F31         | nonvolatile                                      |
| Fields CR2-CR4  | nonvolatile                                      |
| Other CR fields | volatile                                         |
| Other registers | volatile                                         |

Volatile registers are allowed to be mutated without preserving them for the
caller, and cannot be expected to be preserved by the callee. Non-volatile
registers are expected to not be mutated by the end of a callee's execution. To
use non-volatile registers (R14-R31 or F14-F31), all registers from the lowest
on you plan to use all the way through R31 / F31 must be written to the stack
at once. So, it makes sense to use non-volatiles starting at the end of the
range. Of course, these non-volatiles must be restored at the end of the function.

Interestingly, the Linux kernel apparently always stores a pointer to the currently-executing task descriptor struct in R2 (not sure whether this is only in kernel mode though).

According to the document I was referencing, the stack frame must remain doubleword-aligned. Easy way to do this is to make sure when changing the stack pointer, the difference is a multiple of 0x8.

# Basics

Load and stores:

- Start with "l" or "st"
- Use "b" for byte, "h" for halfword, "w" for word
- Use nothing for register base + immediate offset, use "x" for register base + register offset
- "x" means "indeXed"

# Instructions

CraftSpider:
Anything with opcode 4 is GC unique I know (first 6 bits of instruction are opcode)
(More stuff is GC unique besides this though, see psq_st for example)

addi A, B, imm
adds signed 16-bit imm to value of B and assigns to A, UNLESS B is r0, then
just treats r0 as if it has value 0 (assigns imm directly to A)

cmp instructions:
l means unsigned, i means immediate (else register)
so there's cmp cmpl, cmpi, cmpli

condition register has 8 fields each with 4 bits, so CR0 means 1st 4-bit field
bits store result of comparison (equal, greater than, less than, "summary overflow")

the "w" in cmpw means "compare word"; 64-bit ppc has "d" instead for comparing double-word (8 bytes)
include "w" in instruction so don't have to specify it as an operand

+ or - at end of branch instruction means "likely" or "unlikely"
  (ppc-inject doesn't support this, maybe it always writes the instruction as "unlikely")

ppc-inject has a shortcut: cmpw r3, r4 means cmpw cr0, r3, r4

# Gekko float stuff

See page 107 of gekko_user_manual.pdf

# GNU Assembly

Can mark symbols with `@h`, `@l`, or `@h` to get the high, low, or high-arithmetic part of the symbol's address:
For example:

```asm
lis r5, debugTextColor@ha
lwz r5, debugTextColor@l(r5)
```

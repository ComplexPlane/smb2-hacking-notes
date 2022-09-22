# Initial stuff

Code in main.dol at 0x800651d8 overwrites a larger arena hi with the default arena hi, so unless we use ppcinject we can't use apesphere to make the game uses a bigger arena from the start

But it's okay, we can manually allocate the game heaps in the extra space

Region of extra memory for us to use at the moment: 
Start: 0x814D2E20
End: 0x87fedb20

Unfortunately these regions aren't included in OSInitAlloc(), so we might need to redirect 

# Post-IPL memory rewriting hacks

0x80000028 (physical memory size) needs original memory size (0x01800000)
0x80000034 (arena hi) needs original arena hi (0x817edb80)
0x80000038 (fst location) also needs original arena hi
0x800000ec (debug monitor location) needs address of end of memory (orig mem size + 0x80000000) (optional?)
0x800000f0 (console simulated memory size) needs original memory size
0x800000f4 (DVD BI2 location) needs original location (0x817ebb80)
0x81201490 (somewhere in apploader) needs original arena hi
0x81300004 (somewhere in IPL) needs original arena hi

IPL memory at 0x87febb90 needs to be moved to 0x817ebb90 (size 0x14470)

All this stuff should probably be DCFlush'ed / ICInvalidate'd

# News

Apparently new version of Dolphin has a slider to let you change the memory size? Maybe it'd work better (aka without seemingly overwriting memory addresses I set?)
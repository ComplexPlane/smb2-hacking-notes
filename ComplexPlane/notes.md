# Constants

DOL entry point file offset: 0x140
DOL entry point memory location: 0x80003140

main_loop size: 0x2dc7cc
main_loop start: 0x80270100
main_loop end (exclusive): 0x8054c8cc
main_loop entry point (_prolog): 0x80270558

smb2 main.dol entry point: 0x80003140
  smb2 main.dol code begins at 0x80003100 however

unrelocated main_loop free space:
size: 0x623a0 bytes (almost 400 KB)
start: 0x279c8c
end (exclusive): 0x2dc02c
mem start: 0x804e9d8c
mem end: 

Start of main_loop.rel in smb2.iso: 0x5617ebe4

SDA is 0x801A0840

Zephiles:

> the stack has a size of 0x10000
> free word: 0x800044c0
> as far as I know, every GameCube game has a static table in memory that contains a bunch of rfi functions, and theres some free space in between each one, so you can store stuff in those areas

## Game IDs

SMB1 NTSC-U: GMBE8P
SMB2 NTSC-U: GM2E8P
Banana Blitz NTSC-U: RSME8P

## Stack

Start address: 801a9f88
  As assigned in `__init_registers`

801a9fa0: where `mkb2.str` appears to be loaded
  Very close to after top of stack

# SMB1/2 Debug mode

0x40210 is a function that writes 0 to the debug address, change it from
38000000 to 38000003 (li 3) and it writes 3 instead (main_loop)

X+Y completes stage instantly. If in story mode stage select, it beats world instantly

## Test mode

Part of debug mode, accessible on main screen

https://tcrf.net/images/0/09/MonkeyBall2-Debug2.png

# ppc-inject

compile ppcinject on Arch: `ghc -dynamic PPCInject.hs`

ppc-inject does not support underscores in names
Can make more than one function "after" a certain function (they'll stack), but initial function cannot be empty

# Nintendont

- Use "native input" setting for proper controller inputs!
  - Noticable difference when holding neutral in monkey ball
- For cheats, place <gamecode>.gct in /codes/ and enable cheats in settings
- For memcard emulation
  - File with name of <first_4_chars_of_gamecode>.raw in /saves/ is card in slot A
  - <first_4_chars_of_gamecode>_B.raw is slot B

# gz notes

## homeboy error

[homeboy.bin: No such file or directory · Issue #41 · glankk/gz](https://github.com/glankk/gz/issues/41)

## gcc compile options

Example:

```
mips64-gcc -c -MMD -MP -DPACKAGE_TARNAME='gz' -DPACKAGE_URL='github.com/glankk/gz' -DF3DEX_GBI_2 -DZ64_VERSION=Z64_OOTMQJ -DWIIVC -std=gnu11 -Wall -ffunction-sections -fdata-sections -O1 -fno-reorder-blocks src/gz/menu_button.c -o obj/gz/oot-mq-j-vc/menu_button.c.o
```

-c: compile without linking (producing .o files)

## glankk (author) on Discord

i don't know how well documented super monkey ball is, or if there is a modding community around it
if there isn't much existing documentation to work with you will probably have to start out doing some research of your own
when i started gz there was enough documentation to at least get something working
gz has a main function just like a normal c program, except it's called "_start" and is called multiple times (once per frame)
control is passed to gz's main function by hooking the call to the game's logic function
there are some other hooks as well, but that's the important one
if you want to learn more about how it works i'd recommend downloading the source code, compiling it for yourself and tinkering with it
you can ask questions in the discord as well
for gamecube, i know there's a melee hack called 20xx, but i don't know much about it
i wrote a pastebin about gz's injection mechanism that may interest you; https://pastebin.com/FK1mWWVU

# shibboleet

TRK: target resident kernel
"its a modular debugging server for the codewarrior debugger
its pretty useless to us but I label it anyways in my database"

# Monkey Ball 1

Start.dol is loaded starting at address 0x800040e0
Start.dol is in smb1.iso at address 0x18000

Current lives in continue: 80205E62 (signed byte)
Instruction to write decremented life counter: 800170D0
  At 0x12FF0 in Start.dol
  At 0x2aff0 in smb1.iso
Instruction to store initial lives for continue in R0: 80039474
  At 0x35394 in Start.dol
lis r5, debugTextColor@ha
lwz r5, debugTextColor@l(r5)
  At 0x4d394 in smb1.iso
Instruction to write an incremented life counter after 1-up at 0x80039068
  At 0x34f88 in Start.dol
  At 0x4cf88 in smb1.iso

Menu position: 801EEC70

[Practice ROM 0.1.0](https://mega.nz/#!kvQUSIAS!Uvt6KQKOvWE-rsDrqWqmQQ9ljaaRK_qR1ocRqhN5MGI)

# printf / filename strings

0x80110940 (with main_loop.rel loaded): very obviously a printf format string, could help reveal addresses of printf and related functions
  It appears that c filenames and line numbers are formatted with this string in debug messages, so there are a lot of c source filename strings available
  Could probably associate line numbers with c source files this way to get a "minimum line count" in the files

c file strings which appear in main_loop:

```
DEMOPuts.c
adx.c
background.c
bg_bow2_au.c
bg_bubble.c
bg_gear.c
bg_guest.c
bg_lava.c
bg_park.c
bg_pot.c
bg_wat2.c
bg_whale.c
bitmap.c
camera.c
cmp_recplay.c
dialog.c
font.c
load.c
main_loop.c
memcard.c
memcard_mkb2.c
mini_mode.c
mode.c
new_ape.c
nl2ngc.c
preview.c
recplay.c
relocation.c
render.c
scnplay.c
scnplay_fx.c
sound.c
stage_name.c
stcoli.c
window.c
```

for sel_ngc:

```
sel_ngc_rel.c
```

for main_game.rel:

```
game_mode.c
scen_mode.c
scen_disp.c
```

Filenames found with command: `<smb2-orig/files/mkb2.main_game.rel| grep -P '\w+\.c' -z`

0x801444724: "cannot allocate memory"
  Could be used to pin down a malloc() function? It might be a different kind of memory problem though.
0x80110120: bunch of OSHeapCheck error messages

# SMB2 UFO

Value that goes between 0-255 as UFO rotates: 80b44262
Matches values but changing doesn't affect anything: 80b44268

# printf?

First to read a printf string at 804B1820: 800708a4

# Level format stuff

The GMA stores the visual geometry and some render flags
The TPL stores textures
The LZ (stagedef) stores the stage config and collision

# TTYD symbol map stuff

Zephiles:

this is the symbol map from the demo. You have to go to the .text section to see where the actual functions are located in memory
Attachment file type: unknown
MarioSt.MAP
4.80 MB
this is one of the retail versions that was made using that demo symbol map
Attachment file type: unknown
G8ME01.map
431.20 KB
they dont always look that nice though. This is a symbol map file for Twilight Princess. You can see that the names for most functions have a bunch of extra letters and whatnot after them. This is because Twilight Princess was supposedly coded in C++, so the names are all mangled
Attachment file type: unknown
GZ2E01.map
1.93 MB
the only way to know if SMB2 has any symbol maps on the demo discs is to actually get the isos for the demo discs and check each one. You also cant rely very much on sites telling you what contents are on each demo disc, as ive seen multiple examples of them being incorrect

# Codes

Disable music (AR):
003e46c0 00000000

Debug mode (CraftedCart AR code):
(this one seems to be better since it lets you press pause when the game first starts)
185bc473 00000003
005bc473 00000003

Debug mode (Gecko):
28145120 ffef0010
005bc473 00000003
e2000001 80008000

# register save functions

The `_savegpr_31_x` type functions are different from the `_savegpr_31` functions because they also include the rest of a function epilog at the end, unlike the non `_x` variant which just does `blr` after saving the GPRs. This also applies to `_restgpr` type functions as well as the floating-point equivalents.

# Random tips

Remember to use .text and .data to define code and data segments in assembly files when defining data in them, or else I think it'll all be interpreted as code and cause weird crashes...
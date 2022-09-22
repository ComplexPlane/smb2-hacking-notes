# Behavior

A couple RELs (maybe more) contain a string that's printed when they're loaded and purged.
For example, main_loop contains "RELOCATION MAIN_LOOP LOADED" and "RELOCATION MAIN_LOOP PURGED", and sel_ngc contains the analogue.

Although, main_loop never seems to actually be purged, and is only loaded once very early during startup...

## CraftSpider

GCD reads it out into imports.txt (current commit is broken, I just noticed.
I'll fix that ASAP), the imports table is a list of offsets from the start of
one file to a location in another file Basically, when a REL is loaded, it
check what other RELs are loaded. Then it overwrites certain sections of them
with pointers to inside itself I'm not sure how it handles RELs dependent on
each other. I've never really had to dive into the nature of REL import tables

bss: section for statically-defined variables without an assigned value (memory is allocated at runtime)

## TwixNinja

Say that you have a pair of lis/addi instruction that put address 80553970 into
r3. The last 16 bits of the lis will be 0x8055 and the last 16 bits of the addi
will be 0x3970. When you try to replace these instructions without disabling
the relocation data corresponding to the lis/addi, the game will replace the
last 16 bits of the two new instructions in that place with 0x8055 and 0x3970
respectively. Effectively, it will delete your new instructions' kneecaps

If you were to put instructions at the end of an executable section, you'll
have to update a bunch of pointers since you added bytes to the file. We
normally inject code by replacing code starting from an address. That is, the
file size of the REL doesn't change. If we see an lis/addi that loads an
address or see a bl that branches to an address not in the REL, we have
ourselves an overwrite and we then use our fixing programs. What I might do at
some point is since we know that main_loop is always loaded at 0x80270100,
relocation data doesn't really matter, so we disable all of the relocation data
and make the instructions that would be overwritten to write the correct
addresses. We then get a whole bunch of extra space We could put more than
instructions, like some other data. We would get 0x623a8 bytes of space

Complex, we don't know of a lot of free space, but we do know of some. Function
803133cc can be made significantly shorter, provided you don't care about
letting green/reds warp more than 2/3 levels respectively. (As an aside, there
is another hurdle you would have to go through to get arbitrary jump distances
working, specifically the fact that vanilla SMB2 only lets you set arbitrary
jump distances for slot 334, Planets)

Also, Complex, making that function shorter gives us 0x260 bytes
152 instructions

# Unrelocated REL

TwixNinja, for main_loop.rel: "We now have a main_loop with a lot of the relocation data removed. It seems to work, upon playing SMB2 with it. This means that using this REL, we have 0x623a0 bytes of extra space, and we don't have to worry about overwrites when writing code for main_loop. The free space starts at 279c8c and ends before 2dc02c"

# REL locations (SMB2)

800030C8: pointer to first REL

## Story Mode / Challenge Mode / Practice Mode

80270100: id 1 (main_loop.rel)
808f3fe0: id 2 (main_game.rel)

## Main Menu

80270100: id 1 (main_loop.rel)
808f3fe0: id 3 (sel_ngc.rel)

## Start Screen

80270100: id 1 (main_loop.rel)

## Monkey Race

80270100: id 1 (main_loop.rel)
808f3fe0: id 0xa (race2.rel)

# Additional REL

Most "additional RELs" (rels that aren't mainloop) are loaded at 0x808F3FE0L (on heap)

This usually ends up being `0x808F3FE0`

HOWEVER, if the OPTION or TEST MODE menu options are chosen in debug mode while the initial rolling ball AV cutscene is playing, the respective `option` and `test_mode` RELs will instead be loaded at 0x80912dc0

Also, it seems like there's apparently no rhyme or reason for it, but sections of RELs won't necessarily be loaded directly after the rel header at 0x808f3fe0

# bss sections

mkb2.main_game.rel: 8097f4a0
mkb2.sel_ngc.rel: 80949ca0
mkb2.main_loop.rel: 8054c8e0
mkb2.race2.rel: 809234e0
mkb2.mini_fight2.rel: 80939ac0
mkb2.pilot2.rel: 80910bc0
mkb2.rel_mini_billiards.rel: 80942dc0
mkb2.rel_mini_bowling.rel: 8091e840
mkb2.golf2.rel: 80942ce0
mkb2.boat.rel: 80928980
mkb2.shooting.rel: 809a5740
mkb2.dogfight.rel: 809150c0
mkb2.rel_mini_futsal.rel: 809369a0
mkb2.baseball.rel: 809b06a0
mkb2.tennis.rel: 8092caa0
mkb2.sel_stage.rel: 808f6fa0
mkb2.test_mode.rel: 8093bb20
mkb2.option.rel: 80925b40

# Section offsets

* All MAIN sections appear to have their memory addresses correct in Ghidra
* `mkb2.main_loop._data3`: actually loaded 0x10 later, at 0x80444160
* `mkb2.main_game_.data0`: actually loaded 0x4 later
* `mkb2.main_game_.data1`: actually loaded 0x4 later
* `mkb2.main_game_.data2`: actually loaded 0x4 later, at 0x80914ab8
* `mkb2.main_game_.data3`: actually loaded 0x4 later, at 0x80915678
* sel_ngc stuff seems to be loaded correctly
* race2 stuff seems to be loaded correctly
* golf2 text0 loaded at 0x808f40b0, NOT at 0x808f40b8
* golf2 data0 actually at 0x8092cbb8
* golf2 data1 actually at 0x8092cbbc
* golf2 data2 actually at 0x8092cbc0
* golf2 data3 happens to be in correct place already (0x8092dd88)
* mini_fight2.text0 loaded at 0x808f40b0
* mini_fight2.data0 loaded at 0x80921fc8
* mini_fight2.data1 loaded at 0x80921fcc
* mini_fight2.data2 loaded at 0x80921fd0
* mini_fight2.data3 loaded at 0x80922b28
* pilot2.text0 loaded at 0x808f40b0
* pilot2.data0 loaded at 0x809078e8
* pilot2.data1 loaded at 0x809078ec
* pilot2.data2 loaded at 0x809078f0
* pilot2.data3 loaded at 0x80908220
* boat.text0 appears to be loaded correctly at 0x808f40b8
* boat.data0 loaded at 0x80918950
* boat.data1 loaded at 0x80918954
* boat.data2 loaded at 0x80918958
* boat.data3 loaded at 0x80919b98
* shooting.text0 loaded at 0x808f40b0
* shooting.data0 loaded at 0x809341a8
* shooting.data1 loaded at 0x809341ac
* shooting.data2 loaded at 0x809341b0
* shooting.data3 loaded at 0x809361c8
* mini_futsal.text0 loaded at 0x808f40b0
* mini_futsal.data0 loaded at 0x80920030
* mini_futsal.data1 loaded at 0x80920034
* mini_futsal.data2 loaded at 0x80920038
* mini_futsal.data3 loaded at 0x80921440
* dogfight.text0 loaded at 0x808f40b0
* dogfight.data0 loaded at 0x8090b040
* dogfight.data1 loaded at 0x8090b044
* dogfight.data2 loaded at 0x8090b048
* dogfight.data3 loaded at 0x8090bc08
* baseball.text0 loaded at 0x808e8390
* baseball.data0 loaded at 0x8090fee0
* baseball.data1 loaded at 0x8090fee4
* baseball.data2 loaded at 0x8090fee8
* baseball.data3 loaded at 0x80914048
* tennis.text0 loaded at 0x808e8398
* tennis.data0 loaded at 0x80911158
* tennis.data1 loaded at 0x8091115c
* tennis.data2 loaded at 0x80911160
* tennis.data3 loaded at 0x809124d8
* sel_stage.text0 loaded at 0x808f40b0
* sel_stage.data0 loaded at 0x808f5730
* sel_stage.data1 loaded at 0x808f5734
* sel_stage.data2 loaded at 0x808f5738
* sel_stage.data3 loaded at 0x808f5780

# OSLinkFixed

- This function apparently lets you mess with the relocation data section of memory after the REL is loaded?
- Only provided in later SDK versions (I think the late 2004 / patched 2004 version has it)

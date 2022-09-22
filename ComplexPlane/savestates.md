# Naive read-write region saving

After saving/restoring before first instruction of main game loop, these errors show up in console, one line printed per second:

```
05:46:937 Core/HW/EXI/EXI_DeviceIPL.cpp:306 N[OSREPORT]: 	[warning]	GP WAIT Timeout...
05:47:953 Core/HW/EXI/EXI_DeviceIPL.cpp:306 N[OSREPORT]: 	[warning]	GP WAIT Timeout...
05:48:971 Core/HW/EXI/EXI_DeviceIPL.cpp:306 N[OSREPORT]: 	[warning]	GP WAIT Timeout...
05:49:988 Core/HW/EXI/EXI_DeviceIPL.cpp:306 N[OSREPORT]: 	[warning]	GP WAIT Timeout...
```

After a certain amount of time, the game may resume normally at the "old state". Then, after 10 seconds or so, it may freeze, seemingly entering an infinite loop inside the main game loop (is the main game loop looping, or is the loop inside it? idk).

You may also see a "An error has occurred. Press the power button to turn off the Nintendo Gamecube" error, which is pretty funny.

The "GP WAIT Timeout" errors are from the `gp_wait` function in Ghidra, and I believe have something to do with the time base register jumping forward instantaneously (since that isn't saved/restored) and the game's rendering / render profiling code.

# Dynamic analysis approach idea

Saving all of the mutable DOL/REL sections takes 3 MB alone, and that's not even counting heaps and who knows what else we may need to save. Unless we create a fork of Nintendont that lets us safely access the Wii-only memory (se `nintendont.md`), we only have somewhere between 350 and 400 KB to work with (don't remember the exact amount). However, it's a lot of RE work to determine the exact meaning and representation of all game state and thereby know what precisely to save and what not to save. So, perhaps a dynamic analysis approach is something that should be looked into.

For the DOL/REL data/bss sections, it's pretty likely that not every value changes while a stage is being played. We could use dynamic analysis to determine which values / ranges of values are actually changing to build a profile, and then only save those.

For heaps, it's more complicated. We'd need to maybe find which code in general is associated with heap blocks which do and/or don't change, and know to save / ignore certain blocks. The heap structure can be completely different depending on the stage loaded however. It might be necessary to dig into the semantics of heap usage a bit heavily to do this. And this is not to mention the fact that new heap blocks can be allocated / modified as a result of background loading from the disc... still no clue how to deal with that.

## Heap Analysis

Idea for an approach for heap analysis: figure out which heap-allocation points allocate heap blocks which mutate while the state is being played. Once these points are labelled, we can hook these points to store a list of pointers to heap blocks which should be saved when savestates are made.

The issue with this however is that a single call point to `OSAlloc` or whatever may be called by many different functions, and may return a variety of mutating and non-mutating heap blocks. So maybe the key is in finding the parent functions which are ultimately responsible for allocating heap blocks which eventually mutate.

Also none of this accounts for heap blocks which may be created dynamically as the stage is played. I believe this happens a little for the stage heap right as the level starts, the replay heap when the level ends, and perhaps other heaps (or other memory locations?) as stuff is loaded from the disc in the background.

# Interrupts

We will likely need to (read: should probably?) disable interrupts when creating and saving states. According to Gamecube docs, interrupts can only be safely disabled for 100 microseconds without the potential of malfunction. We'll see if that's really the case I suppose.

# Naive savestate further investigation

* See if rewriting GXCopyDisp to top of gx_finish_frame saves a frame of latency
* Try savestating the locked cache as well
  * Would probably involve doing savestates inside apesphere itself, may need to cheat and use wii memory or something somehow
  * Could also try to use CARD apis (the analog would be using emulated nintendont memcards)
    * But idk if that would cause trouble having interrupts disabled for long
    * And also it might be weird trying to savestate CARD state while using CARD APIs
  * Alternatively, could tell `g_init_locked_cache` to use real memory instead of locked cache
    * There's already a flag for this in the game, but it allocates from the arena instead and I probably don't want to screw with this, so I'll probably just rework it a bit to make it use a buffer inside of apesphere
  * Might want to try using GXAbortFrame to clear graphics pipeline and settings after loading state
    * Maybe do this for both graphics FIFOS to make sure they're all clear?

# GX pipeline clearing

Maybe the GX FIFOs just need to be cleaned out before continuing to render? After all, after loading a state, there should still be a FIFO full of drawing commands that say how to draw a frame of a non-existant game state which is given to the GP

`GXAbortFrame` seems like the way to go about doing this, and it will also supposedly reset all GPU state in general, so that's cool

As an additional measure, it appears that the initial state of `prev_gx_settings` is all 0xFF (-1), so I can also memset it after loading a state, which should ensure that the game makes no false assumptions about what the current GX settings are

May want to `GXAbortFrame` on both FIFOs just to be safe? Not 100% sure tho. Actually, it may only be necessary to do this on the now-mostly-complete CPU FIFO which will soon be given to the GP in the mainloop; this would have to be done slightly after ApeSphere's `run()` function so another hook may be required. OR, we could just clear the CPU FIFO before giving it to the GP, which might actually be ideal, since the GP will never start processing potentially bad graphics commands.

## Using GXAbortFrame

Unfortunately GXAbortFrame does not appear in SMB2, so we're going to have to port it from the SDK library or another game.

Might be best to get it from another game so the relocations will be correct.

Note though that we may need to both use `GXAbortFrame` on the current FIFO attached to the GP to prevent it from being drawn, as well as clear the CPU FIFO manually perhaps?

May want to read a bit more in the programmer's guide about `GXAbortFrame` and controlling CPU FIFO again?

## FIFO clearing

Maybe the only way to do this (which is kind of hacky) is to overflow the CPU FIFO so the read and write pointers are equal, so it's effectively empty again...

Alternatively, we could create a new temporary GXFifoObj inside ApeSphere, point it to the old CPU FIFO buffer, and attach it, then do the same for the real GXFifoObj last attached to the CPU.

On the other hand, it might be best to not actually call `GXAbortFrame` since certain settings like `GXSetDispCopySrc` may need to be retained, and instead simply reset the settings cache

Actually, maybe a large part of the issue here is that we are saving and restoring the old GXFifoObj structs in main memory! If we're restoring them, they're probably referring to a bogus section of the FIFO buffers. Might be smart to completely reset the graphics FIFOs when loading a state.

## Results

Savestates seem to always be loaded now without the graphics freezing / pipeline / crashing issue!! It seems like completely clearing out the FIFOs (resetting the fifo object read/write pointers such that they referred to empty fifos) did the trick.

Unfortunately the game often crashes some period of time after loading states. Part of the reason seems to do with sound; the sound often becomes glitchy after loading a state, and sometimes when freezes occur, it's inside `event_tick_sound`. However, the game seems like it can also freeze elsewhere, like inside `event_tick_mouse` ?? Not sure if it's just related to sound or not though. Perhaps worth trying to completely disable sound with a DIP switch or something to see if it makes a difference (and to be fair, I was expecting sound to not work perfectly out-of-the-box). 

Also, I'm not sure why my breakpoint inside the conditional that handles an X press to resume a state fires twice when resuming a state, almost as if the statestate itself contains an X button down press (which it shouldn't... I'm only pressing Z, although maybe the inputs bitmap is set to 1s by the time the save-state breakpoint hits).

Have seen crash in `maybe_zeroes_monkey` both inside `event_tick_mouse` and outside it
  Specifically inside 80046180?

# ARAM

Might be able to use ARAM to save/load states? It's 128 MiB of "slower RAM" often used as a disc cache, but can be written to directly. However, using it seems a little complicated as I'm not sure how it might conflict with how it's currently being used in the game (for loading stages, sounds, other stuff?)

# Cache Saving

Maybe it would be good to write the restored memory back to physical memory? For hardware to play nicer?

# Scratch

Timebase storage: 80003538

00 5b db d3 dc 39 0d c0
00 5b db d3 e3 d5 a3 ef

Hijacking OSTime / OSTick doesn't seem to fix gp_wait problem

005bdbd9 19660397

# No DOL Saving

I wanted to see what would happen if I just avoided saving/restoring the DOL memory and just mainloop/maingame, if only to not overwrite internal musyx sound state data for example. I assumed the game would quickly become a very inconsistent mess and crash quickly, but I was dead wrong! Saving/loading a state on the same stage, assuming you never retry or die or anything, seems to always load without crashing! The music is still messed up though, but that's expected; I assume it still plays _something_ though since the game is just potentially using musyx in an inconsistent way, but musyx state isn't corrupted or inconsistent with the sound hardware.

Crashes still can happen when retrying, dying, or some point after doing these, and perhaps obviously if the stage is reloaded from scratch or another stage is loaded. I still should try some stuff to figure out _exactly_ when crashes can happen, for example, would they happen if I save/load stages within a stage without dying/retrying, and then try to back to main menu and try to play the game normally?

I have a feeling a lot of crashes have to do with ARAM, and loading stuff from disc into/out of it. MusyX uses ARAM and the game uses it to cache disc files I believe for some things (but no idea exactly what it's doing). Since I don't save/load ARAM it would make sense that if ARAM changes, stuff can become inconsistent.

## Tests in Labyrinth

Saving only mainloop and heaps, no maingame or dol:

* Restoring a state only works if the control stick is held in the correct direction before loading the state
* Works after retrying the stage
* Game may crash, possibly due to screwed up audio

Makes me think maybe there's some sort of controller input buffering happening in the main.dol data/bss regions. It appears that inputs are read by MAIN and stored in MAIN's data/bss, so should investigate that further.

0x80145120 - 0x80145340 potentially contains all input data, possibly some other irrelevant data in there tho (size 544)

0x80146600 appears to be an address where controller inputs (PADStatus?) is read directly by the SI controller by an interrupt?
It seems like, even if I savestate this value, after I play the game for one frame after loading a state at the mainloop breakpoint, it reverts back to the controller input when the breakpoint was first hit for one frame. This might explain why savestating the controller values didn't seem to be working. In that case, it's probably not that big of a deal, since you'd expect the user to be holding the right input going into a savestate in the first place anyway. It's just something to be aware of when using dolphin I suppose.

Something strange happens if I only save ball, camera, and "something related to level interpolation": assuming I'm holding the same input, loading the savestate on labyrinth the first time I play the stage causes a bonk on the front of the block, but on subsequent retries, loading a state causing me to bounce over the block. Weird.

---

It seems like only capturing mainloop's BSS is necessary to effectively load a state for a static stage. Well it crashes sometimes but that might be related to sound.

Alkso interesting to note, which bananas are collected appears to not be in the stage heap because they're restored even when I don't capture the stage heap.

# Bisect Tests

Bisecting only the left half of mainloop's BSS and always capturing the right half of mainloop's BSS (since it seems like _something_ from both halves is always necessary)

```
Positive regions
mainloop_bss0_part2 0x805BB600 0x8062A32C
main_game_main_heap 0x808F3FC0 0x80ADAE00
main_game_stage_heap 0x80ADAE00 0x80DFAE20
main_game_bg_heap 0x80DFAE20 0x8102AE40
main_game_char_heap 0x8102AE40 0x814AAE60
main_game_replay_heap 0x814AAE60 0x814D2E80

Negative regions

Final regions
mainloop_bss0_part2 0x805BB600 0x8062A32C
main_game_main_heap 0x808F3FC0 0x80ADAE00
main_game_stage_heap 0x80ADAE00 0x80DFAE20
main_game_bg_heap 0x80DFAE20 0x8102AE40
main_game_char_heap 0x8102AE40 0x814AAE60
main_game_replay_heap 0x814AAE60 0x814D2E80

Trying left region: [0x8054C8E0, 0x805BB600)
Trying right region: [0x80000000, 0x80000000)
Trying left region: [0x8054C8E0, 0x80583F70)
Trying right region: [0x80583F70, 0x805BB600)
Trying left region: [0x8054C8E0, 0x80568428)
Trying right region: [0x80568428, 0x80583F70)
Trying left region: [0x8054C8E0, 0x8055A684)
Trying right region: [0x8055A684, 0x80568428)
Trying left region: [0x8054C8E0, 0x805537B0)
Trying right region: [0x805537B0, 0x8055A684)
Trying left region: [0x8054C8E0, 0x80550048)
Trying right region: [0x80550048, 0x805537B0)
Trying left region: [0x8054C8E0, 0x8054E494)
Trying right region: [0x8054E494, 0x80550048)
Trying left region: [0x8054C8E0, 0x8054D6B8)
Trying right region: [0x8054D6B8, 0x8054E494)
Trying left region: [0x8054D6B8, 0x8054DDA4)
Trying right region: [0x8054DDA4, 0x8054E494)
Trying left region: [0x8054DDA4, 0x8054E11C)
Trying right region: [0x8054E11C, 0x8054E494)
Trying left region: [0x8054DDA4, 0x8054DF60)
Trying right region: [0x8054DF60, 0x8054E11C)
Trying left region: [0x8054DF60, 0x8054E03C)
Trying right region: [0x8054E03C, 0x8054E11C)
Trying left region: [0x8054E03C, 0x8054E0AC)
Trying right region: [0x8054E0AC, 0x8054E11C)
Saving neither region alone works. [0x8054E03C, 0x8054E0AC) and [0x8054E0AC, 0x8054E11C)
```

Bisecting the right half of mainloop bss. It converged on a pretty large region, so there's probably still some disparate chunks of relevant data.

```
Positive regions
magic_region1 0x8054E03C 0x8054E11C
main_game_main_heap 0x808F3FC0 0x80ADAE00
main_game_stage_heap 0x80ADAE00 0x80DFAE20
main_game_bg_heap 0x80DFAE20 0x8102AE40
main_game_char_heap 0x8102AE40 0x814AAE60
main_game_replay_heap 0x814AAE60 0x814D2E80

Negative regions

Final regions
magic_region1 0x8054E03C 0x8054E11C
main_game_main_heap 0x808F3FC0 0x80ADAE00
main_game_stage_heap 0x80ADAE00 0x80DFAE20
main_game_bg_heap 0x80DFAE20 0x8102AE40
main_game_char_heap 0x8102AE40 0x814AAE60
main_game_replay_heap 0x814AAE60 0x814D2E80

Trying left region: [0x805BB600, 0x8062A32C)
Trying right region: [0x80000000, 0x80000000)
Trying left region: [0x805BB600, 0x805F2C94)
Trying right region: [0x805F2C94, 0x8062A32C)
Trying left region: [0x805BB600, 0x805D7148)
Trying right region: [0x805D7148, 0x805F2C94)
Trying left region: [0x805BB600, 0x805C93A4)
Trying right region: [0x805C93A4, 0x805D7148)
Trying left region: [0x805BB600, 0x805C24D0)
Trying right region: [0x805C24D0, 0x805C93A4)
Trying left region: [0x805BB600, 0x805BED68)
Trying right region: [0x805BED68, 0x805C24D0)
Trying left region: [0x805BB600, 0x805BD1B4)
Trying right region: [0x805BD1B4, 0x805BED68)
Saving neither region alone works. [0x805BB600, 0x805BD1B4) and [0x805BD1B4, 0x805BED68)
```

Bisecting the left half of "mainloop bss magic region 2": converges on something tiny!

```
Positive regions
magic_mainloop_bss_region1 0x8054E03C 0x8054E11C
magic_mainloop_bss_region2_part2 0x805BD1B2 0x805BED68
main_game_main_heap 0x808F3FC0 0x80ADAE00
main_game_stage_heap 0x80ADAE00 0x80DFAE20
main_game_bg_heap 0x80DFAE20 0x8102AE40
main_game_char_heap 0x8102AE40 0x814AAE60
main_game_replay_heap 0x814AAE60 0x814D2E80

Negative regions

Final regions
magic_mainloop_bss_region1 0x8054E03C 0x8054E11C
magic_mainloop_bss_region2_part2 0x805BD1B2 0x805BED68
main_game_main_heap 0x808F3FC0 0x80ADAE00
main_game_stage_heap 0x80ADAE00 0x80DFAE20
main_game_bg_heap 0x80DFAE20 0x8102AE40
main_game_char_heap 0x8102AE40 0x814AAE60
main_game_replay_heap 0x814AAE60 0x814D2E80

Trying left region: [0x805BB600, 0x805BD1B2)
Trying right region: [0x80000000, 0x80000000)
Trying left region: [0x805BB600, 0x805BC3D8)
Trying right region: [0x805BC3D8, 0x805BD1B2)
Trying left region: [0x805BC3D8, 0x805BCAC4)
Trying right region: [0x805BCAC4, 0x805BD1B2)
Trying left region: [0x805BC3D8, 0x805BC74C)
Trying right region: [0x805BC74C, 0x805BCAC4)
Trying left region: [0x805BC74C, 0x805BC908)
Trying right region: [0x805BC908, 0x805BCAC4)
Trying left region: [0x805BC908, 0x805BC9E4)
Trying right region: [0x805BC9E4, 0x805BCAC4)
Trying left region: [0x805BC908, 0x805BC974)
Trying right region: [0x805BC974, 0x805BC9E4)
Trying left region: [0x805BC974, 0x805BC9AC)
Trying right region: [0x805BC9AC, 0x805BC9E4)
Saving neither region alone works. [0x805BC974, 0x805BC9AC) and [0x805BC9AC, 0x805BC9E4)
```

Bisecting the right half of "mainloop bss magic region 2":

```
Positive regions
magic_mainloop_bss_region1 0x8054E03C 0x8054E11C
magic_mainloop_bss_region2 0x805BC974 0x805BC9E4
main_game_main_heap 0x808F3FC0 0x80ADAE00
main_game_stage_heap 0x80ADAE00 0x80DFAE20
main_game_bg_heap 0x80DFAE20 0x8102AE40
main_game_char_heap 0x8102AE40 0x814AAE60
main_game_replay_heap 0x814AAE60 0x814D2E80

Negative regions

Final regions
magic_mainloop_bss_region1 0x8054E03C 0x8054E11C
magic_mainloop_bss_region2 0x805BC974 0x805BC9E4
main_game_main_heap 0x808F3FC0 0x80ADAE00
main_game_stage_heap 0x80ADAE00 0x80DFAE20
main_game_bg_heap 0x80DFAE20 0x8102AE40
main_game_char_heap 0x8102AE40 0x814AAE60
main_game_replay_heap 0x814AAE60 0x814D2E80

Trying left region: [0x805BD1B2, 0x805BED68)
Trying right region: [0x80000000, 0x80000000)
Trying left region: [0x805BD1B2, 0x805BDF8A)
Trying right region: [0x805BDF8A, 0x805BED68)
Trying left region: [0x805BD1B2, 0x805BD89E)
Trying right region: [0x805BD89E, 0x805BDF8A)
Trying left region: [0x805BD1B2, 0x805BD526)
Trying right region: [0x805BD526, 0x805BD89E)
Trying left region: [0x805BD526, 0x805BD6E2)
Trying right region: [0x805BD6E2, 0x805BD89E)
Trying left region: [0x805BD6E2, 0x805BD7BE)
Trying right region: [0x805BD7BE, 0x805BD89E)
Trying left region: [0x805BD7BE, 0x805BD82E)
Trying right region: [0x805BD82E, 0x805BD89E)
Trying left region: [0x805BD82E, 0x805BD866)
Trying right region: [0x805BD866, 0x805BD89E)
Trying left region: [0x805BD82E, 0x805BD84A)
Trying right region: [0x805BD84A, 0x805BD866)
Trying left region: [0x805BD82E, 0x805BD83A)
Trying right region: [0x805BD83A, 0x805BD84A)
Saving neither region alone works. [0x805BD82E, 0x805BD83A) and [0x805BD83A, 0x805BD84A)
```

## Testing other stuff

Saving these three regions seems to make loading a physics deterministic savestate always work! At least in world 10...

Doesn't work on curve bridge for some reason... non-deterministic wall collision behavior seems to occur on curve bridge, spinning top, bumpy, randomizer walls...

Maybe it has something to do with the direction the character is facing like bites was talking about?

Maybe it's a combination of both. For example on Spinning Saw I see much more wall non-determinism than on cross floors (spinning saw has a fast-animating component on it)

Saving/loading heaps appears to not be doing the trick either... so it must be time for another mainloop bss bisect!

# Another shot

Known good sections which work so far, at least on the curve bridge frames test:

```
{"mainloop_data0", 0x803dd600, 0x4},
{"mainloop_data1", 0x803dd604, 0x4},
{"mainloop_data3", 0x80444160, 0x806bc},
{"mainloop_bss0", 0x8054c8e0, 0xdda4c},
{"main_game_main_heap", 0x808f3fc0, 1994304},
{"main_game_stage_heap", 0x80adae00, 3276832},
{"main_game_char_heap", 0x8102ae40, 4718624},
```

Seems to not work with no char heap (but with main heap, game heap)
Seems to work with no main game heap, but with char heap and stage heap
Doesn't work with only char heap!

So we've learned, it seems to require at least the char heap and stage heap in some capacity. Maybe only the stage heap due to ball velocity jitter due to stage animation.

So, it seems that saving the char heap, stage heap, and the entire mainloop bss section works. But not just saving the original three mainloop .bss sections works, so there must be some other additional part of mainloop's .bss that I missed. Time to do another bisect.

Current regions that work:

```
m_leftRegion{"mainloop_bss0_smth", 0x8054c8e0, 0xdda4},
{"mainloop_bss0_part2", 0x805bb600, 0x6ed2c},
{"main_game_stage_heap", 0x80adae00, 3276832},
{"main_game_char_heap", 0x8102ae40, 4718624},
```

Also just realized that if I had to pick one stage to test on, it should probably be Ex 9 Construction.
It's moving, has a pause strat that depends on precise edge collisions, and has the weird non-deterministic retry behavior.

Also with the above save sections and Expert Ex 9, after a couple retries the game seems to crash inside `load_ape`, which deals with the char heap. Don't know if that's something to do with me saving/restoring the char heap, something to do with this stage, something to do with saving/loading so much of mainloop's BSS, etc.. if I just save/restore the character heap however, I never get a crash.

I think I'd like to modify the memory bisecting tool to automatically break in half larger continuous blocks to be bisected individually (meta-bisection?), as doing it manually is annoying and it feels increasingly necessary.

Current regions:

```
[0x805E9650, 0x805E9688) (size 0x38)
[0x805E9688, 0x805E96C0) (size 0x38)
[0x805E9270, 0x805E934C) (size 0xDC)
[0x805E934C, 0x805E942C) (size 0xE0)
[0x805E942C, 0x805E9508) (size 0xDC)
[0x805E9508, 0x805E95E4) (size 0xDC)
[0x805BC974, 0x805BC9AC) (size 0x38)
[0x805BC9AC, 0x805BC9E4) (size 0x38)
[0x805BC66C, 0x805BC6A4) (size 0x38)
[0x805BC6A4, 0x805BC6DC) (size 0x38)
[0x805EA1B4, 0x805EA1C0) (size 0xC)
[0x805EA1C0, 0x805EA1D0) (size 0x10)
[0x805BD830, 0x805BD83C) (size 0xC)
[0x805BD83C, 0x805BD84C) (size 0x10)
[0x805EA208, 0x805EA2E4) (size 0xDC)
[0x805EA2E4, 0x805EA3C0) (size 0xDC)
[0x805E90B4, 0x805E9190) (size 0xDC)
[0x805E9190, 0x805E9270) (size 0xE0)
```

After finding these regions I tried expert extra 9 again with aiai, but I got more sprite problems...
Something funny is definitely going on with sprites

... eventually boiled down those sections to three small physics-related sections and the rest were sprite-related

# Sprite stuff

Start looking specifically into saving/restoring sprite data directly?
Actually, it almost looks like if I just don't touch any of the sprite info it should be fine, like: saving _some_ sprite info may cause inconsistency requiring saving more sprite info

# Bananas / switches

First bisect, saving the left four bananas on Bumpy:

```
[0x805D4D54, 0x805D4D54) (size 0x0)
[0x805D4D54, 0x805D4D58) (size 0x4)
[0x805D4EA0, 0x805D5590) (size 0x6F0)
[0x80ADAE00, 0x80DFAE20) (size 0x320020)
```

The last region doesn't seem to matter?

First address to write to the single 0x4 word: 0x8031741c
Second addr: 0x80315628

The four 32-bit-size values that affect the four bananas on the left side of bumpy respectively:

```
0x805D4D54
0x805D4F70
0x805D50D8
0x805D5240
```

Distances between these are 360 or 540... smallest struct size that would make sense is 180?

`event_tick_item` is what appears to be iterating over these structs...
and we can see by a pointer incrementing, there appears to be a struct of size 0x5a, which is 90 (not a multiple of 4 but ok!)

0x805bc504 seems to be a byte array per-banana that seems to represent "is this banana visible?"
Does not seems to represent "has this banana been collected" because you can toggle it on/off before collecting,
then collect banana, then try to toggle it on and banana won't reappear, and collecting banana doesn't change the value
It's referenced in function 0x802b065c, which also references a "sprite status array" of a kind... maybe it's related to the banana model's drawing status?

Hm, it seems the banana state array might actually be 256 elements of size 180, rather than 512 of 90... even though `event_tick_item` has a loop that iterates over them in increments of 90...

Aha okay, `event_activate_item` (0x803153f8 GC) provides some clear evidence for this, that there's 256 total and each is 180 bytes

# Switch savestates

It appears that switches resume the state of their item group / animation group counterparts for the most part...
But making a savestate right in from of the switch on Switches and resuming from it produces non-deterministic behavior: sometimes resuming the savestate will finish with the switch pressed, sometimes the switch will not be pressed (get unpressed from resuming the "unpressed" state from the item group at just the wrong time?)

Might be OK to leave off for now?

# Score

Actual score number seems to be restored, but on-screen display of score isn't affected
Score sprite might only update when score _increase_?

# Sparks

```
[0x8054E1C0, 0x8054E1CC) (size 0xC)
[0x8054E1CC, 0x8054E1DC) (size 0x10)
[0x8054E1A4, 0x8054E1B0) (size 0xC)
[0x8054E1B0, 0x8054E1C0) (size 0x10)
[0x805BB600, 0x8062A32C) (size 0x6ED2C) (mainloop bss part 2)
```

Found small region: 0x8054E1A4 - 0x8054E1DC (size 56)
Actual size 0xc0 (192)?
Actual address 0x8054e1a4

Bisecting the mainloop bss part 2 region:

```
[0x805BE678, 0x805BE834) (size 0x1BC)
[0x805BE834, 0x805BE9F0) (size 0x1BC)
[0x805BED68, 0x805BFB40) (size 0xDD8)
[0x805BFB40, 0x805C091C) (size 0xDDC)
[0x805BE9F0, 0x805BED68) (size 0x378)
```

Combined: 0x805BE678, size 0x22a0
Struct size of 176?
Start of array, 0x805be7e4 ?
It's the Effect list

---

OK now we know a bit more about effects are represented in memory...

Maybe we could disable the effects event for a frame? Or figure out whatever is generating sparks and stop it for a frame?

The streaks may depend on the other small region of memory that isn't the effects list... hmm

## Effect Array

Appears at 0x805be7e4
With struct size 176 each
512 elements
(see `event_activate_effect`)

# Pause Menu

Trying to load a state that was created while the game is paused doesn't seem to restore accurately.
I tried all the memory regions I could think of to bisect (all heaps, all main game regions, all mainloop regions, all dol regions) and couldn't find it. So, I decided to bisect: `{"literally_everything", 0x80000000, 0x1800000}`

This gives the address 0x805BCA3C (size 0x4)
I thought maybe it'd be something in mainloop prolog's stack, but no, it's a field in the first ball struct labelled `g_intflag` at the moment

It seems like this has something to do with the ball's state (duh)... the first bit potentially represents "is the ball touching the ground"
... and this would explain why curve bridge frames weren't working, the ball is "not touching the ground" on 59.61, but it _is_ 1-2 frames afterwards!
Probably worth looking into this value more

Hm, saving and restoring the bitfield with apesphere seem to not work though...
Maybe the first bit is being erroneously set after loading the savestate and before it's read... but why would it work in the memory bisector and not apesphere? They both save/load from the first instruction of mainloop
Oh... it's because I was also saving _all_ of the heaps in the bisector
Well, I guess there's also something tied to a heap as well, maybe something else in the Ape struct?

I bisected while additionally saving the stage heap by default, and found this address in the chara heap: 0x81080044

After trying to restore a savestate in a free-falling state, it appears there's yet _another_ thing in the chara heap that's important.
Oh wait, it's _not_ in the chara heap?!?!? AHHHHHHGHGH
TL;DR, it seems the pause menu really fucks with everything, I really need to test more with saving from a paused state and restoring on a non-paused state

---

Did a little bisect on the pause menu, while completely saving the sprite and sprite status arrays implicitly.
Produced two regions:

### 0x8054DCA8 size 0x1c

seems to control the state of the menu itself, i.e. which item is selected, how many total items there are, a repause framecounter?

True size may be 56 (looking at pausemenu-related references)

### 0x805BC474 size 0x4

seems to just contain a bit (bit 4 aka 0x8) which controls whether the game is "locked" or not (whether can control monkey or not)
Setting to 8 when unpaused just freezes the game and prevents it from being paused again
Locking at 1 does nothing when game is unpaused, but trying to pause makes the screen dim but doesn't actually pause the game or open the menu
  Seems like setting any bits other than bit 4 does the same thing

### General stuff

0x8054dcc4 might also be good to save if it's not part of the first region here

So I'm guessing the pause menu animation must be controlled by the sprites themselves then? Since the two regions don't seem to have anything to do with animation, moreso just fundamental state
There only seems to be a single sprite object in the sprite array that represents the pause menu, judging by the sprite status list...
Maybe the Sprite pointers inside the sprite structure correspond to a linked list, and the pause menu has sub-sprites inside of it? 

On Fluctuation, after a retry and the GO sprite disappears, the pause menu sprite is at 0x805ea1bc (21 * 208 + 0x805e90ac)

Since during the bisect I saved the entire sprites array and no other sprites appear active when the pause menu opens up, this must be the only "sprite" associated with the pause menu?
So in theory I should be able to just save this sprite... but I need to know how to identify it

Maybe the way I could just do it for now is by what I think is the disp function pointer inside the struct: at 0x38, this is 0x8032a4bc for the pause menu

# Sub Mode Restoration

We want to be able to restore savestates during a goal, fallout, or timeout submode, so the level doesn't need to be retried before using savestates again.

For restoring after goaling, this seems to work when saving mainloop bss and the character heap. Saving the character heap seems to prevent a condition where the monkey blasts off instantly after going through the goal.

For this first bisect, I'm saving effects, sprites, main mode, and sub mode implicitly. I'll also save party ball and goaltape state, which I found earlier when looking at stage loading code.

For the first half of mainloop bss we have: 0x80553970 (size 0x4)
Appears to be the "ball mode" according to a comment in Ghidra, seems to affect how the camera follows the ball

And in the second half we have: 0x805E02F8 (size 0x4)
Seems to be controller whether the party ball is open...
aaaand it's in a stage object, yep

Bisecting the character heap, we get: 0x81080208 (size 0x4)
Corresponds to what is currently the `flag1` field on the Ape struct in ghidra

Seems like those three values pretty much do the job for goal and fallout

## Replay prevention

I can just lock the submode frame counter after a RINGOUT or GOAL submode has been reached.
The value is initially 270 for RINGOUT (see 0x808f6f44 GC in maingame, `smd_game_ringout_init` in ghidra)
And it's 360 for GOAL (see 0x808f53a0 GC in maingame)
And for TIMEOUT it's 120

# Crash on Domino

Creating a savestate on Domino seems to crash. `git bisect` says it was caused by this commit: 10b0cd1e81029c6e0c8ad26dfaa1bc73e321ec47
And looking up some addresses on the traceback seems to point to something item-related...

Indeed seems to only crash if I copy the saved copy of the item list back into the game's item list, but I have no idea why

Bombsquad notes that the stagedef is quite large at 1.8mb, wonder if it's some memory-size-related issue
... It was because I was overflowing my fixed array of itemgroups because my assert was being suppressed in dolphin

# Party ball / goal tape

It'd be good to restore the party ball state as sometimes playing the stage involves manipulating the party ball...
Also, restoring the goaltape to _at least a default state_ would be good. Specific state probably doesn't matter much, but it should be there visually and provide the knockback force it usually does

# Savestate reproduction issue on Guillotine?

A bisect of all memory gave: 0x8054EDC8 (size 0x4)

# Ape animation issue

If you load a savestate that was made in the air during the blast-up post-goal animation, the ape stays in a frozen animation.
Saving the entire ape struct does seem to at least unlock the animation after spending a little time on the ground, but there must be more data needed then and I don't know if I want to save the entire ape struct
It doesn't seem to happen consistently, maybe I'll just ignore it for now...

# Minimap

Bisecting gives: 0x80494400 size 0xc0
Although somewhere along the way, fallout detection appears to have stopped working...?

Minimap scale: 0x80494474 (size 0x2)
Minimap rotation: 0x80494476 (size 0x2)

Addresses that write to minimap scale when shrinking during fallout:
0x8033b19c

Found a value here that appears to control whether the minimap is shrinking/expanding/etc: 0x80494444 
This address writes to it when falling out: 0x8033b87c
Ah, seems there's a function here that sets the minimap mode: 0x808f6f8c
Called by `smd_game_ringout_init` which makes sense

## Mode Transition

* After loading a stage fresh, `smd_game_first_init` will call the minimap init function with 4
  * This is likely to initialize the minimap, but the actual mode is still set to 0 (hidden)
* During the ready submode, `smd_game_ready_tick` will set the mode to 3 (expand) once on a certain frame
* Once it's expanded completely, `event_tick_minimap` will set the mode to 2 (shown)
* When you fallout, `smd_game_ringout_init` will set the mode to 1 (shrink)
* Once it's shrunk completely, `event_minimap_tick` will set the mode to 0 (hidden)
* When you retry, `event_dest_minimap` will set the mode to 0 (hidden)
  * This happens regardless of when you retry
* After exiting the game, `event_dest_minimap` will set the mode to 0 (hidden)

## Code idea

Hook the minimap mode setting function:
If a SHRINK mode is requested, ignore it
If an EXPAND mode is requested, re-initialize the minimap and let it expand
This way, the minimap isn't closed after fallout and loading savestates doesn't touch it, but it still zooms in on retry
In theory at least...

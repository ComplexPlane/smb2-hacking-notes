# Stagedef file notes

Level itself (main header) has global list of foreground, background models
Each collision header (representing an _item group_) has a list of level models
Models are basically names of the GMAs I believe (the "visual" model)
Collision headers (item groups) have "collision triangles" in addition to the model

# Expert extra 9

When savestate memory bisecting on expert extra 9, I get these two 32-byte values in the stage heap:

## 0x80B896C4

Appears to be 4-byte counter which controls the animation frame of the cylinder
Pressing switches and holding A when spinning in the level affects the speed/direction it changes
Spinning in the level makes it advance 1 at a time, holding A while spinning in makes it advance 2 at a time
Freezing the value doesn't affect the stage during spin-in for some reason
  Figures, during stage spin-in it's only written to, never read

During gameplay:

29:25:913 Core/PowerPC/BreakPoints.cpp:245 N[MI]: MBP 802c6470 ( --- ) Read32 0000017b at 80b896c4 ( --- )
29:26:166 Core/PowerPC/BreakPoints.cpp:245 N[MI]: MBP 802c6584 ( --- ) Write32 0000017c at 80b896c4 ( --- )
29:26:806 Core/PowerPC/BreakPoints.cpp:245 N[MI]: MBP 802c6588 ( --- ) Read32 0000017c at 80b896c4 ( --- )
29:26:051 Core/PowerPC/BreakPoints.cpp:245 N[MI]: MBP 802d346c ( --- ) Read32 0000017c at 80b896c4 ( --- )
29:40:527 Core/PowerPC/BreakPoints.cpp:245 N[MI]: MBP 802d78e8 ( --- ) Read32 0000017c at 80b896c4 ( --- )

## 0x80B896E4

# Fluctuation

0x80B4A984 GC, size 0xc80 (3200)

Potentially 19 structs of size 160 each starting around 80b4b164
One short in each struct definitely seems to correspond to an animation keyframe

# Quick Turn

0x80B0CAE0 GC, size 0x4b0 (1200)
End: 0x80b0cf90

Seems to have simliar structs with keyframes as Fluctuation, seems like the keyframe controls both the position and rotation.

Pointers to each "stage object state" structs containing the keyframes, as encountered by function 0x802c635c GC:
(Each corresponds to a moving triangle on the stage)

0x80b0cb60
0x80b0cc00
0x80b0cca0
0x80b0cd40
0x80b0cde0
0x80b0ce80
0x80b0cf20

# Item group animation state struct

Pretty confirmed this thing exists

Size 160

0x805bd97c GC is a pointer to an array of these allocated on the stage heap, one for each collision header in the stagedef

Neat, 0x805bdad0 is an array of these structs of same length as the other pointer, used as a temporary copy for view stage!
Makes sense, so view stage can show the original view of the stage

This copying occurs in `event_activate_view` (0x80912c64 GC) and `event_deactivate_view` (0x80913120 GC)

0x802c6c98 GC seems to be the function for initializing the item group anim state

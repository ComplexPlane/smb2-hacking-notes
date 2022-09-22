# Locked cache

Gamecube has a "locked cache" which is basically a region of cache memory that you manually control; you can read/write directly to it with normal instructions, or you can call special functions to shuffle memory between it and main memory with DMA. It's optional though, and has to be enabled.

The locked cache starts at 0xE0000000 and is not backed by main memory (unlike normal cached virtual addresses like 0x80......)

SMB2 uses the locked cache for... some stuff

Appears to store some gx state and settings in some of it, including `graphics_info` and `prev_gx_settings`

Also it seems to potentially be used for a fast memcpy?

In general it seems to potentially be used for a lot of graphics stuff. There's a pointer to the start of the locked cache stored at 0x801992ec and it's referenced by a _ton_ of stuff, most of it seemingly graphics-related.

Seems like the first 12 words of the locked cache potentially correspond to a 3x4 matrix. See extmath_set_a_mtx_rotate_quat (0x80063030)

In fact, it seems that the first 12 words are used as the "current working matrix" which is used when calling extmath functions

# Special values

At 0xE0000094: "matrix stack pointer", used by the extmath push and pop mtx functions
At 0xE000019c: 0x3f800000


There seems to be a kind of "sprite" abstraction in the game

At 0x805e90ac in-game there appears to be an array of 80 sprite structs
Function 0x8031f47c GC seems to be consistent with 80 as the number

0x805bc694 GC seems kind of important... it's the only other address besides the sprite structs that seemingly needed to be saved to not have messed up sprites on screen...

It's 80 bytes long, might have something to do with sprite status

A simple related function to study perhaps: 0x8031f328 GC

# Structure

Sprites live in a tickable list similar to effects, stage objects, and items.
Unlike those however, they appear to have an embedded status value and function pointers.
They definitely have a tick and dest function, but I'm not sure what the third function pointer is, and I don't think it's "init". It might be "disp", like effects.
In SMB1 things seem a little different... sprites might not contain function pointers?

Is `g_font` really a font? Maybe it's an ID, or a "category"? I saw some function with some error about a sprite category.

Is this some sort of string table for sprite names? 0x80456ba0 GC

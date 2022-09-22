# Overview

Don't know how to better describe it yet, but the game often uses a pattern of a struct combined with an associated set of functions consisting of at least an init function, main function, and destroy function. The structs are usually found in a dedicated array in mainloop's BSS.

Sometimes each instance of a struct type contains function pointers to its respective init, main, destroy etc functions. "Event" and "Background" objects do this for example; I believe it's done when there's only one single instance of an "object" per "type"... which makes sense because otherwise you'd be duplicating function pointers across objects of the same type and wasting memory.

Other times the functions appear in dedicated function tables. The usual pattern seems to be a list of string pointers for the names of the potential "object type types", then a list of init functions, then a list of main functions, etc.

There seem to be tickable types for events, sprites, backgrounds, effects, items, and more. Loads of these functions are labelled with a consistent naming convention in SMB1 already, so it's just a matter of replicating the labels in SMB2.

# Naming stuff

Still need to commit to a naming convention for the function types at least in SMB2 anyway; in the SMB1 decompile, yoshimaster's labels use `init` for the function that's called once at the start of the object's use, `main` for the function that's called every frame when the object is in use, and `dest` for the function that's called before the object isn't in use ("destruct"? "destroy"?). Currently the corresponding names in the SMB2 decompile are generally "activate", "tick" and "deactivate"; it'd be nice to shorten them at least I think.

I also think "tick" is a bit more clear than "main", but judging by the game strings and more of yoshimaster's smb1 labels the game appears to use "init" and "main" function names for mode-related functions, so maybe it'd be more consistent to just call the tick functions 'main' functions.

Also I cringe at just resorting to calling these things "objects" as that's so overloaded, even though it is tempting... what about "tickable"? Idk, might use this as a stand-in name until something better is figured out

Also in the smb1 decompile I notice the naming convention for event functions compared to, say, effect or bg functions is backwards:

`ev_item_init` compared to `effect_main_bubble` or `bg_init_sun`

I'll probably use the latter convention as it appears to be used more often in smb1 and goes from broad to specific

Rename returngates to wormholes? goalbags to party balls?

# Tickable Group Status

Some arrays of tickable struct objects appear to have a small metadata struct associated with them. It seems to contain (at least):

* The total number of struct objects in the array
* The number of currently active objects in the array
* A pointer to an byte array representing status values of a sort: 0 seems to mean the object is completely inactive, non-zero values means they're active?

potential name "TickableArrayMeta"? Maybe worth checking the SMB1 decompile to see if it's got a name for this

Function which seems to at least partially initialize these: 0x802b065c GC
Another relevant function: 0x802b0858 GC
And another: 0x802b0990 GC
  RESUME HERE

Addresses that write to the 2nd index var in tickable list meta struct:
0x802b096c <- writes every frame
0x802b09c8 <- writes when there's new effects being created

# Stage Objects

Pretty sure array of stage objects is here: 0x805e0154 GC

## TODO 

<!-- function tables -->
<!-- stobj struct? -->
<!-- stobj type enum -->
<!-- label stobj type in struct -->
<!-- Resume here to finish goaltape / goalbag loading function: 0x8031aa5c -->
union for the "pointer to extra data" field?

# PhysicsBall

is second argument of stobj functions a ball struct? ... no, seems to be a "PhysicsBall" struct
  "PhysicsBall" stuff? a certain struct that's passed to `stobj_coli` functions
  Go back to 0x802bbf04 GC
  Then you can properly define StobjColiFunc's arguments

A lot of things hinting it may have size 96

# TODO

<!-- See if effect struct contains "type" -->
<!-- effect enum? -->
  <!-- Label effect function ptr arrays -->
<!-- finish labelling effect function names -->
<!-- consider reconsiling naming convention to something like "init", "main", "dest", "disp" etc -->
  <!-- Modify ghidra symbol import plugin to rename existing symbols? -->
<!-- do same for  -->
  <!-- bg?  -->
  <!-- item?  -->
  <!--   function tables -->
  <!--   item type enum -->
  <!--   label item type in struct -->
  <!-- all other tickable stuff? -->
<!-- stage object stuff -->
<!-- go thru status array / object metadata struct initialization function -->
<!-- replace "item_group" with "itemgroup" to distinguish from items -->
  <!-- See if the byte array contains STATs? Look for where it's printed maybe? -->
  <!-- Another extremely relevant function that I think might search thru status array to find free tickable object slot: 0x802b0990 -->
  <!-- Potential name: TickableListMeta -->
<!-- bg enum? -->
Add a few labels to Item struct based on debug menu
There seems to be some evidence of a Camera tickable array? Where is that?
Give better name to "null_all_event_states" like smb1 decompile if possible?
<!-- Rename banana stuff to "Item"? If these aren't bananas in party games -->
mode stuff? (also has init / main in smb2 decompile)
Fix function signatures for effect functions (and other tickable functions)
  Define function typedefs and make the tables arrays of them too
Eventually look into sprite initialization functions? (Like are defined in smb1)
<!-- Better naming for switches that appear on the stage? Using local variables named "switch" could lead to trouble later... -->
Should the arrays of structs be balled "arrays" or "lists"? AHHHH
Are some of the tickable object structs unions? Might explain why they can all be the same size despite have quite different types e.g. the different effect types
  Maybe I'll hold off on providing the arguments for _every_ tickable function until I have a better idea of whether it's a union?

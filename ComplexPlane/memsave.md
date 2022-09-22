memsave class that stores address regions in a single buffer
For memory regions that have a direct position you can save/load to (i.e. not the pause menu tickable sprite which we need to position manually)

pass 1: generate total buffer size
pass 2: actually perform a save state
pass 3: load a savestate

And you only need to write code for this once! No separate code to preallocate buffer, save state, or load state
Want to preallocate a single buffer for this to: 

* Check that we have enough heap space for a savestate before making one
* Avoid heap fragmentation
* Avoid writing separate saving/restoring/serialization/deserialization code

Some additional info will need to be stored per state though that has no direct restore addresses, like:
* Pause menu sprite
* Stage ID
* Savestate date?
* Maybe in the future, certain effects?

(it's not a lot, honestly...)

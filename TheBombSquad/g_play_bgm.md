# SoftStreamStart (formerly g\_play_\bgm)

## Info
This function starts at instruction `0x802a50f0` in GameCube memory. It appears to be called when the current BGM track starts playing.

Calling this function with a BGM ID that matches the currently playing track, with looping state 1, does not cause the track to restart, nor does any new track start playing.

## Parameters
* r4 = track to be played
* r3 = whether or not to restart the track (0 = restart, 1 = don't) (this parameter may have other values, I will refer to it as 'looping state')

## Called by
* Called by `FUN_801cf9d4` (g\_change\_bgm) - after the goal sequence 
	This function passes parameter r4, which it gets from r28  
	... and gets r28 from r3
	* Called by function near `0x808f5fac` (0x804cdf34 in Ghidra)					
		This function passes the parameter r3 to the above function.
		story mode fix: replace 802a5c34 with li r3, 1
		
* Called by `FUN_801cf9d4` (g\_change\_bgm) - when hitting stage select
	Fades out first. Need to fix this...
	* Gets track id from near `0x802748b8`
	* Gets looping state `0x802a5c34`
		
* Called by `FUN_801cf88c` (g\_related\_to\_stage\_music) - when selecting a stage from the selection menu
	
## Misc

Something &s ball_state with 0xffffffe after pressing stage select...


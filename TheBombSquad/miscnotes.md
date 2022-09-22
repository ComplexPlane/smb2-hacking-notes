# Miscellaneous
Just a bunch of random things that aren't really worth their own documents

## Desert Haze

### Functions
* `0x802e4ed4` - an instruction that checks if the theme is desert
* `0x802e4e98` - ditto

### Haze Value
`0x8061ba68` in GameCube memory is a char which affects whether or not the current stage displays specific 'rendefc'. 
In vanilla SMB2, if the world theme is 7 (sand), desert haze will be enabled when this is 1.
If this value is changed mid-game, it will cause reflections on the oasis waterfalls to stop updating the reflection's current view angle. 

## Stage loading function
This function is found at instruction 0x801f0f80 in GameCube memory.

`param_1` refers to the stage number to be loaded. 

If the stage number is not 190, a .gma and .tpl file will be loaded.

`FUN_801a6750` and `FUN_801a6cd0` are two functions which call this function. Both of these print the following string to console:

```
pre_load_stage init:2 now:1
```

## Misc
* FUN_80258cf0 - displays the Perfect popup
* FUN_804cf720 - calls FUN_80258cf @ `0x808f7798`
* FUN_802558b8 - handles displaying the large stage load-in text, e.g. 'MASTER EX #'

* `monkey_flags` correlates directly with POLY_DISP's menu for toggling whether or not to render certain types of models/effects.

* `0x80474219` in GameCube memory stores the current world theme.
* `0x805BC494` in GameCube memory stores the number of bananas *originally* placed on a stage.
* `0x803f5694` in GameCube memory stores the number of bananas *currently* on the stage.
	* This is calculated by FUN_801c052c every frame. SMB1 checks if this is zero every frame, while SMB2 has this check removed.
	


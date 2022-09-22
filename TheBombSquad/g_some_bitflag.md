# g\_some\_bitflag

## Info
This variable is stored at `0x80553970` in GameCube memory.

It appears to correlate with the ball's 'mode', found in the debug display panel 'Ball'. However, the numerical value displayed in the debug screen is not exactly the same as the one stored in memory.

Screwing around with various combinations can produce unusual effects. It can lead to the stage restarting as the 'FINAL STAGE', as well as changing the ID of the next stage.

## Flags
* 0000 0000 0000 0000 - None - the ball is in this state normally, unless the stage is a bonus stage or the final stage in a level.
* 0000 0000 0000 0001 - 'Goal' - level restarts, monkey count not affected
* 0000 0000 0000 0010 - 'Time Over' - level restarts, minus one monkey
* 0000 0000 0000 0100 - 'Fall Out' - level restarts, minus one monkey
* 0000 0000 0000 1000 - Timer stops
* 0000 0000 0001 0000 - Ball sound effects change to SMB1 sound effects. Level animation speed breaks
* 0000 0000 0010 0000 - 'Goal' and 'Fall Out' are not possible. Only way to complete the level is through a 'Time Over'
* 0000 0000 0100 0000 - Game sets this for bonus stages, is checked for drawing 'BONUS STAGE' on level load. No other obvious effect
* 0000 0000 1000 0000 - ?
* 0000 0001 0000 0000 - ?
* 0000 0010 0000 0000 - 'Perfect' - level restarts
* 0000 0100 0000 0000 - ?
* 0000 1000 0000 0000 - Timer stops
* 0001 0000 0000 0000 - Game sets this flag for final stages, is checked for drawing 'FINAL STAGE' on level load. No other obvious effect.
* 0010 0000 0000 0000 - ?
* 0100 0000 0000 0000 - ?
* 1000 0000 0000 0000 - Flags beyond this point don't seem to have any visible effect.

## Combinations
Random combinations I've tested.

* 0xF00 - displays 'TIME OVER', animation for successfully completeing a level, the level restarts with no lost monkeys
* 0xE00 - same as above
* 0xD00 - stops timer, no other effect
* 0xC00 - same as above
* 0xB00 - perfect, but the stage restarts with no lost monkeys
* 0xA00 - same as above
# TODO / ideas

<!-- * Change instructional text to "Up/Down to change world" -->
* Automatically return to main menu when finishing final stage in world
  * Implement this by making the game act like you selected "exit game" when you stage-select on last stage?
  * Change "Stage Select" text to "Finish IW" after beating final stage in world
  * Change exit game scrolling text to custom IW message
<!-- * Make filename animated somehow, like spinning character? -->
  <!-- * Maybe make the name less redundant than just the world name? -->
<!-- * Detect current file selected -->
<!-- * Select world with up/down on stick or dpad (using special filtered binary input values) -->
<!-- * Don't change world if left/right input also detected -->
<!-- * See if anything screwy with any% is going on -->
<!-- * Name file "W01 IW" - "W10 IW" depending on world -->
* Show world-completion-time on final level beaten?
* Sound when changing world?
* Allow picking any monkey for IWs? (or full story runs)
<!-- * Patch iso using dd instead of gcrebuilder on linux -->
* Hold up/down to rapid change
<!-- * Don't mess up non-IW savefiles -->
<!--   * Seems kind of pointless to implement in a practice ROM tbh, not doing this for now -->
* Add artificial lag frames for loading stages?
  * Might want to have a fixed "expected" lag frame counter per stage/world/transition and "add" additional lag frames as necessary

## Bugs

<!-- * Stage select hack doesn't work when not in World 1 -->
<!-- * exit game text hack doesn't work if a party game would be unlocked -->
<!--   * Ignoring by not implementing this for now -->
* "Finish IW" text also in Practice Mode

# Memcard save format (.gci)

[File Structure for GCI File](http://www.surugi.com/projects/gcifaq.html)

0x20a4: start of first story mode block?
0x40: checksum (uint16)?

# Story mode save

Relative to start of .gci for now for first story mode save

0x20a8: filename (6 bytes)?
0x20b8: file playtime (int32)?
0x20bc: file score (int32)

# Story mode file struct

Size: 132 bytes
3 structs repeat in memory, one for each save file

0x8061cc60: start of 1st savefile struct
0x8061cce4: Start of 2nd story mode save file struct
0x8061cd68: Start of 3rd story mode save struct

OFFSETS:

0x4: 4-byte bitfield? Bit in position 0 seems to describe "does this savefile exist?"
0x8: filename for first file
0x15: current world number (int8) (0 means world 1, 9 means world 10)
0x18: playtime for first file in frames (int32)
0x1c: score for first file (int32)
0x24: number of stages beaten in the world (int8)
0x25: int8[10] array of stage ids beaten in the world

# File Select code

0x809072b0: Code that checks if save file struct (pointed to by r26) represents a non-blank save file
0x80907028: Function that draws a save file on the screen
  Arguments:
  r3: pointer to save file struct
  r4: index of save file (0, 1, or 2)
0x80907d5c: Call to 0x80907028 made three times, once per save file struct
0x8031ec70: Seems to call function pointer that draws entire save file UI
  Since it's a function pointer call it could be for drawing something other than save file UI
  Seems to be responsible for drawing all sorts of 2D elements / menu stuff across the whole game!
0x80907914: Seems to draw all file select boxes
  Called once per tick by main_loop
  Not part of main_loop

0x805e90ac: Struct containing pointer to function used to draw ui elements?
  0x805e90e4: address of function pointer described above, in the struct? (offset 0x38 in struct)

0x8001292c: responsible for drawing almost every 2d element on the screen?
  Seems to be called more than once per frame, hmm

# Story mode data select menu

0x809800ed: int8 (0 if a data file is focused, 1 if "copy", "delete", or "erase" is focused)
0x809800ee: int8 (id of story mode file / file operation buttons focused)
  For example, 1 means either save file 2 is focused or "delete" is focused

0x80918130: "Continue the game from the saved point."

## New file screen

Lots of other values nearby when creating and naming new file, like cursor position

0x809800b8: list of 6 pointers, one for each character in filename currently typed
0x809800d0: int8 (number of characters currently typed)

# Story mode pause menu

0x8054DCB4: pause menu cursor position (4 bytes)

Addresses which read:

These three read once in order, couldn't discern what they do yet
0x80273354
0x80273644
0x8027512c

These two read sequentially 6 times
0x8032a89c: Seems responsible for highlighting the currently selected entry
0x8032aaa8: Seems responsible for drawing the monkey face next to the entry

0x802735f4: Adds one to the selection position when menuing down
0x802734a0: Subtracts one from the selection position when menuing down

0x8047F02C: "Stage select" menu text
0x8032a870: Call to draw pause menu text in main_loop?
  r4 seems to be a pointer to the text to draw
  Changes don't seem to update immediately and nop-ing seems to draw a random character sometimes, may not be as simple as I think

## Pressing A

0x80273a48
0x80273ac0
0x8032a8e4 
  Seems called 6 times for each menu entry, but for "How to play", "Stage select", and "Exit Game" it's called 6 times for *every* frame
0x80274574

## 0x80274574

Seems to decide what to do after menu selection is made
If stage select or exit game are selected, but this address assumes item 0 is selected, the fade-out transition will show but the game will be resumed

0x802745ac: runs if "Continue game" selected
0x802745cc: runs if "Retry" selected
0x80274678: runs if "View stage" selected
0x80274770: runs if "How to play" selected
0x80274804: runs if "Stage select" selected
0x802748cc: runs if "Exit game" selected
0x80274988: runs if selected item index not in range [0, 5] 
  This is just the function epilogue

## stage select menu

Cursor position:
0x8054dbbf
0x805e95df
0x8097f727

0x8054dbbf might be a part of struct at 0x8054dbb8

Nearby struct to 

Number of stages beaten in world:
0x805D4B0A (half-word) (only in World 1)
  0x803151a0: Writes incremented beaten stage count after completing a stage
  number of stages beaten in world at 
0x8097F4FE (half-word)
  Written to by: 0x8000343c (writes 0??)
  Written to by: 0x808feb58
0x8097F614 (word)
  Written to by: 0x80003454 (writes 0??)
  Written to by: 0x808ffe98

## Stage status

0x8054DC40: struct of current stage status? not sure
  Word at this address: number of frames remaining to show camera animation before "GO" and post-goal
0x8054dc83: byte representing whether current stage is beaten or not

Another potential address for "stage/world status": 0x8054dbb8
Looking at function 0x80315130 for reference (which increments "number of stages beaten in the world" counter
0x0004: current world? (2 bytes)

0x805d4b08: Array of per-world structs? Each is 56 bytes big according to instruction 0x80315150
In world struct at offset 0x2: half-word that represents how many stages beaten in the world

## Exit game

First line:
0x804c4254: "You received %d Play Points"
0x804c44f4: "Play Point record for this time"
0x804c4290: "You didn't get any Play Points."

Second line:
0x804c42c8: "You now have a total of %d Play Points."
0x804c45e4: "Highest Play Points record"

Third line:
0x804c4308: "Only %d Play Points left\nto unlock the next Party Game!"
0x804c4358: "/bcff8000/You can unlock a party game..."
0x804c43bc: "Earn more Play Points\nto get a gift!"
0x804c46b0: "Do your best to try and beat\nthe high score, okay?"

0x8032a014: call to sprintf perhaps? Called with first two strings above which are format strings
  Format string is passed in r4
  0x80329d90: appears to be function responsible for handling / drawing / prerendering "Do your best" text
0x803dcd1c: Call to this function, with ptr to "Do your best" text in r4

## Fixing "new high score" exit game bug

0x805BCA1C: score
0x8091E7B2: "Current Play Points" format string
0x8091e7a8: "Current Play Points" string including color code at beginning
0x8062A298: Current play points (when in main menu) (word)
0x803dc960: code that runs after selecting exit game and reads current play point count
appears to check if play points are >= 99999 initially

0x80444340: "RELOCATION MAIN_LOOP PURGED\n"

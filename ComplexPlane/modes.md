Game has state-machine states aka "modes"

MainMode and SubMode, abbreviated MD and SMD by the game

There's a main mode roughly for each rel (GAME for main game, SEL for sel ngc, MINI for party games, etc)
And a lot of submodes for each main mode...

Some mode state is stored starting at 0x8054dc30 GC in SMB2 and around 0x803c7dbc in SMB1
In SMB1 it seems there's a "current mode" as well as "requested mode" for main mode and sub mode, probably is a thing in SMB2 too

There's also function tables corresponding to main modes and sub modes... should look into exactly how they work

There's also the "main game mode" aka story/practice/challenge here: 0x8054dc68 GC
Can take other values besides story/practice/challenge tho from the looks of it

Setting main_mode_request and sub_mode_request seems to cause the respective mode to load immediately! The function that seems to take care of this is: 0x80271924 GC

See: [heaps.md · master · CraftedCart / smb2-modding-notes · GitLab](https://gitlab.com/CraftedCart/smb2-modding-notes/blob/master/heaps.md)

So on bootup

mainHeap is created
mainHeap is destroyed and recreated
game heaps are created (including mainHeap)

the AV logo plays

all game heaps are destroyed
game heaps are created again

I really love bananas

all game heaps are destroyed
game heaps are created again

smb2 splash screen

all game heaps destroyed again
game heaps are created again

main menu
I now go an enter a practise mode stage

all game heaps destroyed again
game heaps are created again

yeah you get the point..

# Heap locations

Except for the initial game heaps made during game initialization, it seems that the heaps are allocated between
0x808f3fc0 and 0x814d2e80

(total size 0xbdeec0)

Whether 4 heaps are allocated (main, stage, bg, char) or 5 (main, stage, bg, char, replay), they're contained in this range, the space is just divided between the heaps differently

# CraftedCart

Heap stuff:

RAM 0x8054DC1C MainHeap size
RAM 0x8054DC20 StageHeap size
RAM 0x8054DC24 BackgroundHeap size
RAM 0x8054DC28 CharaHeap size
RAM 0x8054DC2C ReplayHeap size

All next to each other in memory

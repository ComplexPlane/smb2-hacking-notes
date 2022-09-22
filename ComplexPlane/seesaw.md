If an itemgroup is a seesaw, it appears the last field of the itemgroup struct (offset 0x3c) is set to a pointer to a 32-byte seesaw-specific struct allocated on the heap

Itemgroup array on seesaw bridges is at 0x80AED660

The three seesaw structs on seesaw bridges are allocated at:
0x80aedce0
0x80aeddc0
0x80aedea0

These contain another struct of size 120 (which I bet refers to the _actual_ seesaw state, the 32-byte struct seems to mostly contain function pointers)
0x80AEDD20
0x80AEDE00
0x80AEDEE0

0x80145120: lots of interesting and potentially useful values related to controller inputs

There are some bitfields which describe the filtered cardinal direction changes of analog sticks as follows:
Basically, if the control stick moves from neutral to a cardinal direction (like up), a bit will be set.
If the control stick moves back near neutral, the bit will be cleared
The threshold is "fat": the stick must move through a "middle region" for a bit change to occur
  Makes sense; this is conceptually similar to a deadzone and is probably for preventing small movements from rapidly being interpreted as "rapid up/down/up/down" for example
Multiple bits can be changed with diagonals as multiple cardinal directions are affected
These bitfields also appear to have a "previous frame" copy; probably so you can check a couple bits to see if the control stick changed position!

0x80145300: filtered analog inputs (32 bits)
Bit 0-5: blank/unknown
Bit 6: right analog trigger
Bit 7: left analog trigger
Bit 8: c stick up
Bit 9: c stick down
Bit 10: c stick right
Bit 11: c stick left
Bit 12: control stick up
Bit 13: control stick down
Bit 14: control stick right
Bit 15: control stick left
Bit 16-21: blank/unknown
Bit 22: previous right analog trigger
Bit 23: previous left analog trigger
Bit 24: previous c stick up
Bit 25: previous c stick down
Bit 26: previous c stick right
Bit 27: previous c stick left
Bit 28: previous control stick up
Bit 29: previous control stick down
Bit 30: previous control stick right
Bit 31: previous control stick left

0x8014530C: Button bitfield
Bit 0-2: blank/unknown
Bit 3: Start
Bit 4: Y
Bit 5: X
Bit 6: B
Bit 7: A
Bit 8: blank/unknown
Bit 9: Left trigger
Bit 10: Right trigger
Bit 11: Z
Bit 12: dpad up
Bit 13: dpad down
Bit 14: dpad right
Bit 15: dpad left
Bit 16-18: blank/unknown
Bit 19: previous Start
Bit 20: previous Y
Bit 21: previous X
Bit 22: previous B
Bit 23: previous A
Bit 24: previous blank/unknown
Bit 25: previous Left trigger
Bit 26: previous Right trigger
Bit 27: previous Z
Bit 28: previous dpad up
Bit 29: previous dpad down
Bit 30: previous dpad right
Bit 31: previous dpad left

0x80145340: Number of frames since last controller input, not counting small control stick changes (int32)

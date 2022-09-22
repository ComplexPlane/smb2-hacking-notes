Tentative row count: 38
Tentative col count: 54
total cells: 1961
  Also happens to be the size (maybe minus some padding or terminating slots) of the mystery character buffers

Function which draws the semi-transparent debug text background: 0x801d93b4
  Pretty simple function, I think it draws a quad with a color given in memory as a texture parameter or something
  Seems really easy to reuse this to draw rectangles in general and a good example of a simple draw call

Color format for drawDebugTextDrawChar\* functions: 0brrggbbaa (I think)

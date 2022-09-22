# Investigating from high-level loop of main_loop.rel

0x80270558: Function which has the game loop?
0x80270934: Call to 3D render function? (doesn't render 2D graphics)
  White where 3D-rendered stuff normally is when nop-ing
  In Story Mode stage select, balls and stage preview still rendered, but shading is either not present or incorrect
  Something that's strange is that UI elements still seem to be updated after nop-ing, even though they're seemingly redrawn by this call

# TODO, move images to documents somewhere

I'm keeping scrots in `bites-addresses/`

# 8054DC40

anyways, it's a timer in frames that determines how long until the current 'active transition' is over - things like the level lead-in, post-goal waiting and post-goal replay, etc
it always gets set to a certain value then ticks down once per frame until it hits 0 at which point the next thing in the sequence happens

for example, when a level begins it's set to 360 and when you retry it's set to 120
and each time when it hits 25(?), the ball spawns
and when it hits 0 you gain control

you can also set it to 0 to immediately skip post-goal bullshit, and interestingly when you do, the stage select music does not play
the world music continues
which means that the trigger which causes the world music to fade out and to attempt to play the stage select music is based off of this timer

Also, the value itself may control the current progress on the transition
  Might be related to ball drop-in progress on stage drop-in animation for example

# Other stuff

level tilt interpolation rate (float): 803e4d38

> by the way, this value actually controls not just the visual of the level tilt, but also the literal gravity tilt on the ball
> so setting it very low = your control over the ball is very sluggish
> and very high = basically instant

bg animation timescale (double): 80e353dc
stage object animation rate (fps/val) (double): 803e4dd8
stage animrate post dropin (fps/val) (double): 803e4dd8
stage animrate b4 dropin (fps/val) (float): 803e4dc8
minimap size (short): 80494474
minimap rotation (short): 80494476
minimap rotation rate (short) 80494478

# 803e8624

time divisor for showing milliseconds on timer

math is likely: `curr_time % 60 * 100 / val_803E8624`

805EA1CC: affects ready/go text 

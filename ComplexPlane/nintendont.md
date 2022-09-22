# Zephiles

> The difference is that standard Wii mode disables a lot of the Wii's features, thus making it closer to the GameCube in terms of functionality and whatnot. Nintendont also handles time using the Wii's clock rate or whatever it's called, instead of using the GameCube's. So this means there will be inaccuracies if a time is set in one mode and then used in another without being reset first.
> And we've noticed a reduction in lag and load times for TTYD in Nintendont as well, which is why we don't allow it to be used for official runs.
> If you're looking for better accuracy, you could try using Neogamma instead, but that requires installing a modified MIOS, which may or may not have an effect on how the game runs.
>
> ComplexPlaneToday at 4:30 PM
> Haven't heard of neogamma, might check it out
>
> PistonMinerToday at 4:51 PM
> I believe Nintendont runs the PPC at Broadway rate so that would be expected to improve performance when you're CPU-limited
> And the Flipper/Hollywood clock would be faster too

# PistonMiner

> I believe Nintendont runs the PPC at Broadway rate so that would be expected to improve performance when you're CPU-limited. And the Flipper/Hollywood clock would be faster too
> Hollywood clock switch is bit 1 of the Hollywood registers at 0x0d800190 based on reference material. There's presumably another switch to adjust the PPC PLL config
PLL = phase locked loop
It's a way to generate one clock signal from another

# Jasper

MIOS also sets a whole bunch of random flag bits that cause it to run basically like an old GC

# MEM2 access

> Does running under Nintendont also grant the game direct access to the Wii's MEM2 memory?

PistonMiner:

Sorta; I believe all the memory is mapped into its normal location, but Nintendont actually uses almost all of it by default; it fills everything it doesn't need for anything else with a disk cache to speed up loading times. So if you want to use that memory, which I've considered in the past, you are probably either going to have to make some (probably rather intrusive) patches or just straight-up use a modified version of Nintendont. It would be nice to have a "reserve memory for mods" feature which you could just select in Nintendont and which it then exposes via it's "root" structure, but I'm not sure how feasible it would be to get that upstreamed

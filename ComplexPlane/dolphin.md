# Dolphin debug mode

launch like `dolphin-emu -d`
don't use multicore emulation, causes cpu/gpu desync on frame advance
  maybe this isn't a bug on newer versions / git version of Dolphin

Each REL refers to a different part of the game. For instance, iirc main_game refers to the story mode menus and stuff
You can see where they're being loaded in Dolphin debug:

CraftedCart:

View > Show Log Config > Enable everything (Or, well at least enable OSReport
but I usually just press the "Toggle all" button View > Show Log

TwixNinja:
main_game also has stuff with Challenge Mode logic

Option -> Boot to pause useful sometimes?
Often want to pause after relocation main_loop is loaded, else superfluous reads?

# Emulate modified game as directory without rebuilding ISO

TwixNinja:

Double click your ISO in the Dolphin ISO Manager (whatever it's called), select
Properties -> Filesystem. Right click the disc icon and select Extract Entire
Disc
To run: in Dolphin, open sys/boot.dol

To rebuild ISO directly from dolphin's dump, copy '&&systemdata' from Gamecube
Rebuilder's dump under files/ in Dolphin's dump.

# Basic Debugging

To pause the game immediately upon starting, in latest Dolphin, Options -> Boot to Pause

To enable dolphin memory engine to read dolphin memory:

```bash
sudo setcap cap_sys_ptrace=eip Dolphin-memory-engine
```

To find location of REL in memory, search for large strips of hex in rel and cross-reference

Bites:

addresses for variables can be discovered rather easily using dolphin memory
engine, which works similarly to cheat engine but operating strictly within
the emulated game's memory range

if you know the value of a variable, you can search for it specifically - if
there are multiple of the same variable, you can find the specific one you're
searching for by performing repeated searches after you change this variable,
to refine the result

if you want to find instructions or code relevant to these variables, you can
then use memory breakpoints - pause code execution in dolphin when these
variables are either read or written to

these will stop the game at EXACTLY the instruction a particular variable was
read or written, which is useful for finding the code relating to the shit
you wanna change

for instance, to change the 1up count to 50 bananas, i found where the banana
counter was stored in memory, then put a memory breakpoint when it was
written to, and after getting a 1up, a 0 is written
that put me very near the code i needed to change

# Game saves

The game save file for smb2 is 8P-GM2E-super_monkey_ball_2.dat.gci . Set the memory card to "GCI Folder" under Gamecube settings in Dolphin settings to use it.

NOTE: it seems to not matter at all what the save file is named in the dolphin dir.

# Free Mode

Graphics settings -> Advanced -> Utility -> Free Look

# Dolphin RAM dump

Dolphin can apparently dump RAM contents to .raw file, could be potentially useful

# Generate symbol map

Dolphin can find some function symbols in your game for you, and export a map file which can be imported into Ghidra for example. Beward that not all symbols will be correct though: some could be plain wrong, for Wii and not Gamecube, or similar (but still wrong).

See Symbols menu in Dolphin debug mode

# Savefiles

How to deal with savefiles in a good way because I always forget:

* In Config -> Gamecube set Slot A to GCI Folder
* For a complete smb2 save, copy romhack/res/8P-GM2E-super_monkey_ball_2.dat.gci to ~/.local/share/dolphin-emu/GC/USA/Card\ A/
* Copy ApeSphere gci to this folder too to load it with apesphere rel loader

# Windows

## Compiling

In visual studio 2019, I had to select every project and change the compiler to C++17. Shift-clicking helps select many projects faster.

Bombsquad's version: 74b2410d7e

## Preventing Crashes

I was getting some crashes when setting breakpoints related to invalid memory reads, these resolved when disabling dual-core
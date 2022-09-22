# Setup

[Ghidra does not render correctly with GDK_SCALE set (HiDPI display) · Issue #1 · NationalSecurityAgency/ghidra](https://github.com/NationalSecurityAgency/ghidra/issues/1)

May want to consider running with xpra to just scale the whole application with X
  Utility called run_scaled to do this
  Need to install xvfb

CraftedCart: dpi scaling fixed in jre 9
maybe could try installing different jdk/jre versions to try to remedy
For now, setting font size override in support/launch.properties is OK

# Tips

* To search for addresses in RAM that ghidra might not have detected, put cursor on address and Search -> Direct References
  * Useful for identifying function pointers in lookup tables in data section
* If you're trying to rename something and it doesn't appear to work, try right clicking -> commit locals, then try again
* Mark hardware memory regions such as graphics fifo as volatile
* Disable "write" on read-only data sections so the decompiler can use the fact that these values never change
* To prevent faulty function signature detection for save/restore register functions, set them all to take and return void and to inline
  * Re-analyze afterwards
  * Allows function arguments (which were previously detected as return values from set/restore register functions) to be detected correctly
* You can find usages of a struct field by right-clicking the field in the struct editor
* To search in a box for something that _starts_ with a string, it seems like you can just put a space in front of it
* To replace type A with type B: in the data types view, drag B onto A

# New version setup

## Launch config

Copy `support/launch.properties`

## Ghidra Gamecube Loader

https://github.com/Cuyler36/Ghidra-GameCube-Loader

Building with ./gradlew requires the current jdk version to be 11.

```
GHIDRA_INSTALL_DIR=~/build/ghidra_9.1.2_PUBLIC/ ./gradlew
```

On macOS, after installing openjdk@11 with Homebrew, use this to temporarily set the right PATH for ghidra-gamecube-loader:

```
export PATH="/usr/local/opt/openjdk@11/bin:$PATH"
```

## ghidra-gekko-broadway-lang

```
cp -r data/languages/* $GHIDRA_INSTALL_DIR/Ghidra/Processors/PowerPC/data/languages
```

## ghidra-supermonkeyballtools

https://gitlab.com/CraftedCart/ghidra-supermonkeyballtools.git

To fix: "could not find method compile() for arguments [directory 'lib'] on object of type org.gradle.api.internal.artifacts.dsl.dependencies.DefaultDependencyHandler."
Edit C:\Users\ComplexPlane\build\ghidra_9.2.3_PUBLIC\support\buildExtension.gradle and replace "compile" with "implementation"
See: https://stackoverflow.com/a/66910991/864310

## patch diff correlator

https://github.com/threatrack/ghidra-patchdiff-correlator

# Java versions

Ghidra 9.1.1 and below:

Server fails to connect when client has JDK 13.0.2: https://github.com/NationalSecurityAgency/ghidra/issues/1440
  (or sometimes version 11 too)
Client should be 13.0.1 for now I guess
Archive: https://jdk.java.net/archive/

Need to set JAVA_HOME_OVERRIDE in `support/launch.properties` to extracted JDK dir

# Some good settings

## Highlight symbol on left-click instead of middle-click

Edit -> Tool Options -> Listing Fields -> Cursor Text Highlight -> Mouse Button to Activate -> LEFT

## Decompiler display options

* Right click decompiler window -> Properties, scroll down to the checkboxes, most are cool options
  * Current I've checked every box except "Display Namespaces"
* There's more checkboxes under Decompiler -> Analysis on the side, I checked "Use inplace assignment operators"
* Always show struct offsets in hex: Edit -> Tool Options -> Editors -> Structure Editor

## Font

On Windows, Consolas seems to look a lot better than the default Monospace font. Make sure to change it in the console, listing, decompiler, and byte viewer.
I've also tried Fira Code / Fira Mono but they don't look too hot really.

## Listing view

To disable rewriting load/store offset values / register names with symbol names,
go to Edit -> Tool Options -> Listing Fields -> Operands Field -> disable some stuff

## Struct editor

Tool options -> something something -> struct editor -> show offsets in hex

# Ghidra extension dev

[Writing a wasm loader for Ghidra. Part 1: Problem statement and setting up environment / Habr](https://habr.com/en/post/443318/)

GhidraBuild/EclipsePlugins/GhidraDev/GhidraDevPlugin/build_README.txt

## CraftedCart:

Does importing a gradle project not work? (and then RClick the project in the workspace in eclipse, and link with Ghidra)
Also Eclipse can do hot reload if you launch Ghidra in a debugger, though it is java so changing class members and what not isn't allowed with hot reload

## Ghidra discord

"If can't run extension from inside eclipse, just run `gradle buildExtension` and install it to test"

## Reloading extension without having to reinstall extension in ghidra menus

ghidra_supermonkeyballtools extension must be installed first

```
#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

EXTDIR="$GHIDRA_INSTALL_DIR"/Ghidra/Extensions

gradle buildExtension
rm -rf "$EXTDIR"/ghidra_supermonkeyballtools
unzip dist/ghidra*.zip -d "$EXTDIR"
```

Restart Ghidra after running (can't just reopen project unfortunately)

Project can be opened as a Gradle project in any editor that supports it for completion and such

# Data Type Archives

Apparently, you can create separate "data type archives" which contain datatypes you can reuse across projects and files. Yet to figure out how to create one though.

# Diffing

In Listing View, hit the button that looks like two white boxes with a diagonal line between them

# Old Ghidra versions

Warning: running an old Ghidra version can seemingly corrupt your projects, even if you don't open them!

# Equates

Use Equates to label constant scalar values like #define, aka not an enum type. Press E on a scalar to create a new one.

# How to reset a function name to default

In Listing view, press Delete on function name. If it turns into a label with the same name, press Delete again, then press F to create a function.

If if turns into a label and pressing Delete does nothing, it might be because the function name itself appears to have arguments inside parens (thanks Dolphin). Rename the function to remove the arguments and parens, and then perform the above steps.

# Auto create structure

If a variable in the decompile view looks like it may be a struct, you can right-click it and choose "Auto Create Structure" and Ghidra will make a new structure definition and try to fill in fields with data types depending on how fields are accessed. You can also right-click an existing struct type and choose "Auto fill-in struct" to have it set some more types on the existing struct depending on how it's used.

# Version Tracking

It's amazing for matching up symbols from other libraries or other decompiles with your game! Try it!

Things to look into:

<!-- - Upgrade gekko and gamecube-loader plugins for ghidra 9.1.2 -->
<!-- - See, in general, whether the 2001 or 2002 musyx version correlates better with smb2 -->
  <!-- - It seems like the 2002 version matches better than the 2001 version, and much better than the 2004 version -->
<!-- - Load musyx shared library archive as single file and version-track that against smb2 instead of a bunch of individual files -->
  <!-- - Unfortunately it doesn't seem like you can actually import as a single file with a gamecube archive -->
<!-- - Version track the rest of the gamecube sdk -> smb2 to see if it can find data labels -->
<!--   - Correlator doesn't seem like it matches data labels very well... -->
<!-- - Read manual for ghidra version tracking -->
<!-- - Figure out how to actually apply the matches -->
<!--   - What the different flags / checkmarks mean? -->
<!--   - What are implied matches? Seems like these can help find data references? -->
<!-- - Look into the patch diff correlator -->
<!--   - See if this can detect `sndStreamAllocEx` which is slightly bigger in smb2 than in the 2001/2002 musyx versions -->
<!-- - Run reference correlators after running exact correlators _after_ processing all objects in archive? -->
<!-- - Try to figure out how to apply implied matches that are external function / data references -->
<!--   - These seem to fail when automatically applied for some reason, dunno why, important to get them though -->
<!-- - After running exact correlators and non-external applied matches for each archive: -->
  <!-- - For each archive, accept (manually or otherwise) external implied matches -->
  <!-- - For each archive, run reference correlators (and only run them on stuff that doesn't already have an accepted match) -->
  <!--   - Maybe want to run "exact symbol name correlator" and then run reference correlators on stuff that hasn't been accepted -->
  <!-- - Finally, go back through and look for unmatched functions and see if there's something that can be done -->
  <!-- - Figure out how to label array element matches better? -->
  <!-- - Maybe some other time -->
- Version track other SDK libraries for more data labels
  - See if there's a 2002 SDK version / other better matching version than the 2001 version I'd originally used?
  - Do something about unmatched functions? (Window -> Version Tracking Functions -> button near top-right -> show unmatched functions)
<!-- - Version track smb1 -> smb2 -->
<!--   - Nothing new was found! RIP -->
- Maybe eventually, try version tracking another game with symbols against smb2?
  - Useful to disable "automatically apply function/data label on accept" in Tool Options -> Version tracking, then match on exact symbol name, accept them, _then_ run the other correlators while ignoring already accepted matches (so we can ignore pulling in bad/redundant information about already labeled stuff from another game)
  - Need to manually apply markup for future matches though... maybe this isn't the right way to do this?
- extmath
  - Label arguments + return types for extmath functions
    - Also fix functions which do not return
  - Label remaining extmath functions, at least with `g_` prefixes
- Export each version of Ghidra to C source and throw into a git repo
  - Export header separately
  - Hopefully git repo wouldn't get too big?

# Float conversion

> Jasper ⭐Today at 4:56 PM
> There's no hardware instruction for converting u32 -> f32, so this is using some bitwise trickery by setting the floating point exponent so that it's in the "ones" range, and then ORing in the integer into the mantissa section, and then subtracting out 1.0
> There's an explanation in a compiler writer's manual somewhere
> https://cr.yp.to/2005-590/powerpc-cwg.pdf#page=103
> In a 32-bit implementation, you may convert a 32-bit integer to a float-ing-point value as follows. Flip the integer sign bit and place the resultin  the  low-order  part  of  a  doubleword  in  memory.  Create  the  high-order part with sign and exponent fields such that the resulting dou-bleword  value  interpreted  as  a  hexadecimal  floating-point  value  is0x1.00000dddddddd×10D,  where  0xdddddddd  is  the  hexadecimalsign-flipped  integer  value.  Then,  load  the  doubleword  as  a  floating-point    value.    Subtract    the    hexadecimal    floating-point    value0x1.0000080000000×10D  from  the  previous  value  to  generate  theresult.
> It's a clever algorithm tbh
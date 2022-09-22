# Memory offset

Data sections in memory map in Ghidra are sometimes loaded with extra padding between them in the actual game (32-byte alignment?)

For example, mkb2.main_loop.data3 isn't loaded in memory immediately following data2, there's 16 bytes of padding

Current Ghidra plugin reports that data3 is at 0x80444150 when really it's at 0x80444160

# Plugin TODOS

<!-- * Fix data section off-by-16-or-something problems -->
<!-- * Implement BSS sections -->
* Implement heaps if possible
<!-- * Fix REL/DOL file location (probably wrong since it's updating for bss sections) -->
<!-- * Show more information in Convert Address window -->
  <!-- * Module: show exact section name/number -->
  <!-- * Show section read-only/read-write/volatile -->
<!-- * RAM to Ghidra address improvements -->
  <!-- * Support bss/heap obviously -->
  <!-- * When RAM address is ambiguous and asking which module is loaded, only show modules which are possible based on the range -->
  <!--   * Easy way to implement this: filter possible sections based on address, then only prompt user for module when there's more than one possibility -->
  <!--   * This also prevents the user from incorrectly selecting a main.dol/mainloop module when prompted to select "which additional rel is loaded", which currently gives a faulty Ghidra address -->
  <!-- * Support hardware registers (0xcc000000 and above) -->
<!-- * Convert view to table to save space? -->
<!--   * No -->
<!-- * Default address in "jump to address" dialog should be current RAM address, not ghidra address -->
<!--   * Might be better to make it the most recent Gamecube address instead, as not every "current ghidra address" has a corresponding Gamecube address -->
<!--   * This is also more consistent with the normal Ghidra jump-to-address functionality (g) -->
<!-- * Support debug mode RELs: sel_stage, option, and test_mode -->
<!--   * Perhaps not now, would need to handle the fact that option and test_mode rels can be loaded at different locations depending on whether they're activated from initial AV cutscene or not -->


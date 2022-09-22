Warning: old stuff below

# REL loader

- First code line writes string "rel" to somewhere in memory
  - This is the filename of the rel on the card, used to open it
- Hook point defined as 0x8006FE38, this means that loader (nintendont/gecko os) will
  - Write code to loader-defined unused memory region
    - This is apparently often unused ISR addresses
  - Write branch instruction to 0x8006FE38, pointing to where code was written to
  - Add a branch instruction after the code back to the hook point
  - The user should manually run the instruction that the hook point overrode
  - Zephiles: The loaders usually put all of its gecko code work and whatnot in the Exception Vector area of memory, which is 0x80001800 to 0x8003000, as this area of memory is unused. The exact position of the code is dependent on the loader itself, so its technically dynamic in that case

## Card stuff

CARDMountAsync: 800267b0
CARDUnmount: 800269ec
CARDGetResultCode: 800233e4
OSAlloc: 8000a794
OSFree: 8000a7bc
OSUnlink: 80010b8c


#include "vml/patch.h"
#include "vml/alloc.h"
#include "vml/log.h"
#include "mkb2/gamecube.h"

void vml_patch_instruction(void *buffer, u32 offset, u32 instr) {
    *((u32*) (((u32) buffer) + offset)) = instr;
    vml_info("Patched instruction at 0x%08X", (u32) buffer + offset);
}

void vml_patch_buf_inject_func(void *buffer, u32 offset, VmlInjectedFunc func) {
    u32 abs_offset = ((u32) buffer) + offset;
    u32 prev_instr = *((u32*) abs_offset);

    // TODO: Free this memory if we ever unload stuff maybe?
    u32 *hook_code = vml_malloc(12); // Allocate memory for 3 instructions
    /* if (hook_code == NULL) vml_fatal("Could not OSAllocFromArenaLo in vml_patch_inject_func"); */

    hook_code[0] = PPC_INSTR_BL((s64) func - (s64) &hook_code[0]);
    hook_code[1] = prev_instr;
    hook_code[2] = PPC_INSTR_B(((s64) abs_offset - (s64) &hook_code[2]) + 4);

    u32 target = (u32) ((s64) hook_code - (s64) abs_offset);
    *((u32*) abs_offset) = PPC_INSTR_B(target);

    vml_info("Injected function call to 0x%08X at 0x%08X", (u32) func, abs_offset);
}

void vml_patch_ram_inject_func(void *address, VmlInjectedFunc func) {
    u32 abs_offset = (u32) address;
    u32 prev_instr = *((u32*) abs_offset);

    // TODO: Free this memory if we ever unload stuff maybe?
    u32 *hook_code = vml_malloc(12); // Allocate memory for 3 instructions
    /* if (hook_code == NULL) vml_fatal("Could not OSAllocFromArenaLo in vml_patch_inject_func"); */

    hook_code[0] = PPC_INSTR_BL((s64) func - (s64) &hook_code[0]);
    hook_code[1] = prev_instr;
    hook_code[2] = PPC_INSTR_B(((s64) abs_offset - (s64) &hook_code[2]) + 4);

    u32 target = (u32) ((s64) hook_code - (s64) abs_offset);
    *((u32*) abs_offset) = PPC_INSTR_B(target);

    vml_info("Injected function call to 0x%08X at 0x%08X", (u32) func, abs_offset);
}

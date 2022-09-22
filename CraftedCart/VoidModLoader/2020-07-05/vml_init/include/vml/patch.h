#ifndef VML_VML_INIT_PATCH_H
#define VML_VML_INIT_PATCH_H

#include "mkb2/gamecube.h"
#include "mkb2/rel.h"

#define PPC_INSTR_B(target) (0x48000000 + (((u32) (target)) & 0x3FFFFFF))
#define PPC_INSTR_BA(target) (0x48000002 + (((u32) (target)) & 0x3FFFFFF))
#define PPC_INSTR_BL(target) (0x48000001 + (((u32) (target)) & 0x3FFFFFF))
#define PPC_INSTR_BLA(target) (0x48000003 + (((u32) (target)) & 0x3FFFFFF))
#define PPC_INSTR_BLR() (0x4E800020)
#define PPC_INSTR_NOP() (0x60000000)

#ifdef __cplusplus
extern "C" {
#endif

extern RelBufferInfo* vml_main_loop_buffer_info;
extern RelBufferInfo* vml_config_buffer_info;

typedef void (*VmlInjectedFunc)();

/** Replace an instruction at buffer + offset */
void vml_patch_instruction(void *buffer, u32 offset, u32 instr);

/**
 * Injects a function call at location buffer + offset. This will run the replaced instruction after the function
 * returns.
 *
 * @note Don't try to replace instructions which depend on their location (like bl instructions)
 */
void vml_patch_buf_inject_func(void *buffer, u32 offset, VmlInjectedFunc func);

void vml_patch_ram_inject_func(void *address, VmlInjectedFunc func);

#ifdef __cplusplus
}
#endif

#endif

#include "vml/init.h"
#include "vml/rel.h"
#include "vml/patch.h"
#include "vml/alloc.h"
#include "vml/log.h"
#include "mkb2/gamecube.h"
#include "mkb2/display.h"
#include <string.h>

// Define externs
RelBufferInfo* vml_main_loop_buffer_info;
RelBufferInfo* vml_config_buffer_info;

void do_thing(void) {
    /* vml_info("GI %u", graphics_info->frame_count); */
}

extern u32 sub_mode2;

void _prolog(void) {
    vml_info("Void Mod Loader initializing");

    // memset the relocation data
    /* memset(((u8*) buffer_info.rel_buffer) + 0x25471C, 0, 0x880B0 - 4); */

    vml_info("Loading /mkb2.main_loop.rel");
    RelBufferInfo buffer_info = vml_rel_load("/mkb2.main_loop.rel");
    vml_main_loop_buffer_info = &buffer_info;

    vml_info("Initializing VML allocation");
    // Reclaim some memory from instructions in main.dol used to load this REL
    vml_alloc_init((void*) 0x80006840, 0x5BC);
    // Reclaim some memory from relocation data in mkb2.main_loop.rel
    vml_alloc_add_block(((u8*) buffer_info.rel_buffer) + 0x25471C, 0x87910);
    vml_alloc_print_free_list();
    vml_info("%.3f KiB of VML heap available", vml_alloc_get_free_bytes() / 1024.0f);

    /* vml_patch_buf_inject_func(buffer_info.rel_buffer, 0x00000804, &do_thing); */

    RelBufferInfo config_buffer_info = vml_rel_load("/vml/vml_config.rel");
    vml_config_buffer_info = &config_buffer_info;

    PADStatus pad_statuses[PAD_MAX_CONTROLLERS];
    PADRead(pad_statuses);
    if (pad_statuses[0].err == PAD_ERR_NONE && pad_statuses[0].triggerRight > 127) {
        /* void *prev_arena_lo = OSGetArenaLo(); */
        vml_rel_run_prolog(&config_buffer_info);
        vml_rel_run_epilog(&config_buffer_info);
        /* OSUnlink(config_buffer_info.rel_buffer); */
        /* OSSetArenaLo(prev_arena_lo); */
    }

    /* vml_patch_buf_inject_func(buffer_info.rel_buffer, 0x00000490, &load_doom); */

    // Run our main VOID mod REL
    vml_info("Running /vml/void.rel _prolog");
    RelBufferInfo void_buffer_info = vml_rel_load("/vml/void.rel");
    vml_rel_run_prolog(&void_buffer_info);

    // Enable DIP_DEBUG and DIP_DISP
    vml_info("Enabling DIP_DEBUG and DIP_DISP");
    vml_patch_instruction(buffer_info.rel_buffer, 0x00040210, 0x38000003); // li r0, 0x0003
    /* *(((u32*) buffer_info.bss_buffer) + 0x6FB90) = 3; */

    // Call the REL _prolog function - this should not return
    vml_info("Running /mkb2.main_loop.rel _prolog");
    vml_rel_run_prolog(&buffer_info);

    // No need to OSUnlink and free memory and what not if we just panic now
    // Besides we use some of the main.dol memory that was used to get here as heap space
    // so chances are we would be returning to garbage memory anyways
    vml_fatal("/mkb2.main_loop.rel returned from its _prolog function - this is not expected");
}

void _epilog(void) {
    vml_info("Void Mod Loader unloaded");
}


void _unresolved(void) {
    vml_fatal("An unresolved function was called in Void Mod Loader");
}

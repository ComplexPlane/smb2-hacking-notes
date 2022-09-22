#include "vml/rel.h"
#include "vml/log.h"
#include "mkb2/gamecube.h"

RelBufferInfo vml_rel_load(char *file_name) {
    // Try open the file
    DVDFileInfo file_info;
    if (!DVDOpen(file_name, &file_info)) {
        vml_fatal("Could not DVDOpen %s", file_name);
    }

    RelBufferInfo buffer_info;

    // Allocate memory to read in the file
    u32 rel_len_32b_aligned = file_info.length + 0x1F & 0xFFFFFFE0;
    buffer_info.rel_buffer = OSAllocFromArenaLo(rel_len_32b_aligned, 32);
    if (buffer_info.rel_buffer == NULL) {
        DVDClose(&file_info);
        vml_fatal("Could not allocate memory for %s", file_name);
    }

    // Read in the file
    s32 dvd_read_status = DVDRead(&file_info, buffer_info.rel_buffer, rel_len_32b_aligned, 0);
    if (dvd_read_status < 0) {
        vml_fatal("Failed to read %s with status code %d", file_name, dvd_read_status);
        DVDClose(&file_info);
    }

    // Get size of the .bss section
    u32 bss_size = *((u32*) ((u32) buffer_info.rel_buffer + 32));
    if (bss_size == 0) bss_size = 32;

    // Allocate memory for the .bss section
    buffer_info.bss_buffer = OSAllocFromArenaLo(bss_size, 32);
    if (buffer_info.bss_buffer == NULL) {
        DVDClose(&file_info);
        vml_fatal("Could not allocate memory for .bss for %s", file_name);
    }

    DVDClose(&file_info);
    OSLink(buffer_info.rel_buffer, buffer_info.bss_buffer);

    vml_info("Loaded %s at 0x%08X with .bss at 0x%08X", file_name, buffer_info.rel_buffer, buffer_info.bss_buffer);

    return buffer_info;
}

void vml_rel_run_prolog(RelBufferInfo *buffer_info) {
    (*((RelPrologFunc*) ((u32) buffer_info->rel_buffer + 0x34)))();
}

void vml_rel_run_epilog(RelBufferInfo *buffer_info) {
    (*((RelEpilogFunc*) ((u32) buffer_info->rel_buffer + 0x38)))();
}

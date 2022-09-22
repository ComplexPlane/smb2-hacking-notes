#ifndef VML_MKB2_MKB2_REL_H
#define VML_MKB2_MKB2_REL_H

#include "mkb2/gamecube.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MKB2_MODULE_ID_MAIN 0
#define MKB2_MODULE_ID_MAIN_LOOP 1
#define MKB2_MODULE_ID_MAIN_GAME 2

// Yes these are all typedefs to the same type
typedef void (*RelPrologFunc)();
typedef void (*RelEpilogFunc)();
typedef void (*RelUnresolvedFunc)();

typedef struct RelBufferInfo {
    void *rel_buffer;
    void *bss_buffer;
} RelBufferInfo;

// TODO: Not g_ these
extern RelBufferInfo* g_loaded_rel_infos[10];
extern u32 g_loaded_rel_count;

#ifdef __cplusplus
}
#endif

#endif

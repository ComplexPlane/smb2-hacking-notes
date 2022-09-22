#ifndef VML_MKB2_MKB2_REL_H
#define VML_MKB2_MKB2_REL_H

#include "mkb2/gamecube.h"

#ifdef __cplusplus
extern "C" {
#endif

// Yes these are all typedefs to the same type
typedef void (*RelPrologFunc)();
typedef void (*RelEpilogFunc)();
typedef void (*RelUnresolvedFunc)();

typedef struct RelBufferInfo {
    void *rel_buffer;
    void *bss_buffer;
} RelBufferInfo;

#ifdef __cplusplus
}
#endif

#endif

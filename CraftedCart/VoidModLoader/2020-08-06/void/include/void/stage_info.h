#ifndef VML_VOID_STAGE_INFO_H
#define VML_VOID_STAGE_INFO_H

#include "mkb2/gamecube.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*VoidStageTickFunc)(void);
typedef void (*VoidStageOnEndFunc)(void);

typedef struct VoidStageInfo {
    VoidStageTickFunc on_tick;
} VoidStageInfo;

extern VoidStageInfo *void_stage_infos[421];

#ifdef __cplusplus
}
#endif

#endif

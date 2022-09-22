#ifndef VML_MKB2_MKB2_COMMON_H
#define VML_MKB2_MKB2_COMMON_H

#include "mkb2/gamecube.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum Status { /* NULL, INIT, NORMAL, and DEST, and FREEZE seem to be the most common */
    STAT_INIT = 1,
    STAT_INIT_FIRST = 5,
    STAT_DEST = 3,
    STAT_GOAL = 7,
    STAT_GOAL_INIT = 6,
    STAT_NORMAL = 2,
    STAT_RINGOUT = 9,
    STAT_FREEZE = 4,
    STAT_RINGOUT_INIT = 8,
    STAT_MINI_MODE_0 = 11,
    STAT_NULL = 0,
    STAT_MINI_MODE_0_INIT = 10
} Status;

typedef struct Vec3f {
    float x;
    float y;
    float z;
} Vec3f;

typedef struct Vec3s {
    s16 x;
    s16 y;
    s16 z;
} Vec3s;

typedef struct Quat {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Quat;

#ifdef __cplusplus
}
#endif

#endif



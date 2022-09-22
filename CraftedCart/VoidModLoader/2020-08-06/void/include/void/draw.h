#ifndef VML_VOID_DRAW_H
#define VML_VOID_DRAW_H

#include "mkb2/gamecube.h"

#ifdef __cplusplus
extern "C" {
#endif

void void_draw_push_vtx_desc();
void void_draw_pop_vtx_desc();

void void_draw_push_vtx_attr(GXVtxFmt fmt, GXAttr attr);
void void_draw_pop_vtx_attr(GXVtxFmt fmt, GXAttr attr);

#ifdef __cplusplus
}
#endif

#endif

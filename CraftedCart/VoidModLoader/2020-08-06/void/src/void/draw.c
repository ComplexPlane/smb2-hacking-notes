#include "void/draw.h"
#include "vml/log.h"

#define VTX_DESC_STACK_MAX 16
#define VTX_ATTR_STACK_MAX 16

static GXVtxDescList vtx_desc_stack[GX_MAX_VTXDESCLIST_SZ * VTX_DESC_STACK_MAX] = {{1, 1}}; // Init to nonsence to work around cube_code issues
static u8 vtx_desc_stack_used = 0;

static GXCompCnt vtx_attr_cnt_stack[VTX_ATTR_STACK_MAX];
static GXCompType vtx_attr_type_stack[VTX_ATTR_STACK_MAX];
static u8 vtx_attr_frac_stack[VTX_ATTR_STACK_MAX];
static u8 vtx_attr_stack_used = 0;

void void_draw_push_vtx_desc() {
    if (vtx_desc_stack_used == VTX_DESC_STACK_MAX) {
        vml_fatal("void_draw_push_vtx_desc: max stack sized reached");
    }

    GXGetVtxDescv(&vtx_desc_stack[vtx_desc_stack_used * GX_MAX_VTXDESCLIST_SZ]);

    vtx_desc_stack_used++;
}

void void_draw_pop_vtx_desc() {
    if (vtx_desc_stack_used == 0) {
        vml_fatal("void_draw_pop_vtx_desc: min stack sized reached");
    }

    vtx_desc_stack_used--;

    GXSetVtxDescv(&vtx_desc_stack[vtx_desc_stack_used * GX_MAX_VTXDESCLIST_SZ]);
}

void void_draw_push_vtx_attr(GXVtxFmt fmt, GXAttr attr) {
    if (vtx_attr_stack_used == VTX_ATTR_STACK_MAX) {
        vml_fatal("void_draw_push_vtx_attr: max stack sized reached");
    }

    GXGetVtxAttrFmt(
            fmt,
            attr,
            &vtx_attr_cnt_stack[vtx_attr_stack_used],
            &vtx_attr_type_stack[vtx_attr_stack_used],
            &vtx_attr_frac_stack[vtx_attr_stack_used]
            );

    vtx_attr_stack_used++;
}

void void_draw_pop_vtx_attr(GXVtxFmt fmt, GXAttr attr) {
    if (vtx_attr_stack_used == 0) {
        vml_fatal("void_draw_pop_vtx_attr: min stack sized reached");
    }

    vtx_attr_stack_used--;

    GXSetVtxAttrFmt(
            fmt,
            attr,
            vtx_attr_cnt_stack[vtx_attr_stack_used],
            vtx_attr_type_stack[vtx_attr_stack_used],
            vtx_attr_frac_stack[vtx_attr_stack_used]
            );
}

#include "void/init.h"
#include "void/font.h"
#include "void/draw.h"
#include "vml/patch.h"
#include "vml/alloc.h"
#include "vml/log.h"
#include "mkb2/gamecube.h"
#include "mkb2/ball.h"
#include "mkb2/camera.h"
#include "mkb2/display.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dolphin/gx/GXEnum.h"
#include "dolphin/gx/GXGeometry.h"

#include "v/container/vec.h"

extern u8 g_some_texture_data;
static void* void_fb;

static void do_thing(void) {
    /* vml_info("GI %u", graphics_info->frame_count); */

    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTexGens(1);

    GXTexObj tex;
    GXInitTexObj(&tex, &g_some_texture_data, 640, 448, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    /* GXInitTexObj(&tex, void_fb, 640, 448, GX_TF_CMPR, GX_CLAMP, GX_CLAMP, GX_FALSE); */
    GXLoadTexObj(&tex, GX_TEXMAP0);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    GXSetZMode(GX_DISABLE, GX_ALWAYS, GX_DISABLE);

    Mtx44 proj;
    MTXOrtho(proj, -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 20000.0f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);

    GXSetCullMode(GX_CULL_NONE); // TODO: Not require this - it's upside down too

    /* GXInvalidateVtxCache(); */
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR1A1);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    GXClearVtxDesc();

    /* void_draw_push_vtx_desc(); */

    /* void_draw_push_vtx_attr(GX_VTXFMT0, GX_VA_POS); */
    /* void_draw_push_vtx_attr(GX_VTXFMT0, GX_VA_CLR0); */
    /* void_draw_push_vtx_attr(GX_VTXFMT0, GX_VA_TEX0); */

    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    Mtx44 view;

    /* GXBegin(GX_QUADS, GX_VTXFMT0, 8); */
    /* // Black quad */
    /* GXPosition3f32(-1.0f, -1.0f, 0.0f); */
    /* GXColor4u8(0, 0, 0, 255); */
    /* GXTexCoord2f32(0.0f, 0.0f); */

    /* GXPosition3f32(-1.0f, 1.0f, 0.0f); */
    /* GXColor4u8(0, 0, 0, 255); */
    /* GXTexCoord2f32(0.0f, 1.0f); */

    /* GXPosition3f32(1.0f, 1.0f, 0.0f); */
    /* GXColor4u8(0, 0, 0, 255); */
    /* GXTexCoord2f32(1.0f, 1.0f); */

    /* GXPosition3f32(1.0f, -1.0f, 0.0f); */
    /* GXColor4u8(0, 0, 0, 255); */
    /* GXTexCoord2f32(1.0f, 0.0f); */

    /* // Game screen */
    /* GXPosition3f32(-0.7f, -0.7f, 0.0f); */
    /* GXColor4u8(255, 255, 255, 255); */
    /* GXTexCoord2f32(0.0f, 0.0f); */

    /* GXPosition3f32(-0.7f, 0.7f, 0.0f); */
    /* GXColor4u8(255, 255, 255, 255); */
    /* GXTexCoord2f32(0.0f, 1.0f); */

    /* GXPosition3f32(0.7f, 0.7f, 0.0f); */
    /* GXColor4u8(255, 255, 255, 255); */
    /* GXTexCoord2f32(1.0f, 1.0f); */

    /* GXPosition3f32(0.7f, -0.7f, 0.0f); */
    /* GXColor4u8(255, 255, 255, 255); */
    /* GXTexCoord2f32(1.0f, 0.0f); */
    /* GXEnd(); */

    GXInitTexObj(&tex, void_fb, 640, 448, GX_TF_CMPR, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXLoadTexObj(&tex, GX_TEXMAP0);

    for (int i = 0; i < 1; i++) {
        float scale = 1.03f;
        /* u8 alpha = 20 * (1.0f - (i / 50.0f)); */
        /* u8 alpha = 240; */
        float rot = 0.00f;

        MTXIdentity(view);

        // I couldn't find MTXRotRad/MTXRotDeg, so we just create the rotation matrix manually here
        Mtx44 rot_mtx = {
            { cos(rot), -sin(rot), 0.0f, 0.0f },
            { sin(rot), cos(rot), 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f, 0.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
        };

        MTXConcat(view, rot_mtx, view);
        GXLoadPosMtxImm(view, GX_PNMTX0);

        GXBegin(GX_QUADS, GX_VTXFMT0, 8);
        GXPosition3f32(-1.0f, 0.0f, 0.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(0.0f, 0.5f);

        GXPosition3f32(-1.0f, scale, 0.0f);
        GXColor4u8(255, 255, 255, 240);
        GXTexCoord2f32(0.0f, 1.0f);

        GXPosition3f32(1.0f, scale, 0.0f);
        GXColor4u8(255, 255, 255, 240);
        GXTexCoord2f32(1.0f, 1.0f);

        GXPosition3f32(1.0f, 0.0f, 0.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(1.0f, 0.5f);

        GXPosition3f32(-1.0f, -scale, 0.0f);
        GXColor4u8(255, 255, 255, 240);
        GXTexCoord2f32(0.0f, 0.0f);

        GXPosition3f32(-1.0f, 0.0f, 0.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(0.0f, 0.5f);

        GXPosition3f32(1.0f, 0.0f, 0.0f);
        GXColor4u8(255, 255, 255, 255);
        GXTexCoord2f32(1.0f, 0.5f);

        GXPosition3f32(1.0f, -scale, 0.0f);
        GXColor4u8(255, 255, 255, 240);
        GXTexCoord2f32(1.0f, 0.0f);
        GXEnd();
    }

    /* void_draw_pop_vtx_attr(GX_VTXFMT0, GX_VA_TEX0); */
    /* void_draw_pop_vtx_attr(GX_VTXFMT0, GX_VA_CLR0); */
    /* void_draw_pop_vtx_attr(GX_VTXFMT0, GX_VA_POS); */

    /* void_draw_pop_vtx_desc(); */

    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);

    GXSetTexCopySrc(0, 0, 640, 448);
    GXSetTexCopyDst(640, 448, GX_TF_CMPR, GX_FALSE);
    GXCopyTex(void_fb, GX_FALSE);
}

static BOOL is_in_teleport_zone = FALSE;
static void test(void) {
    /* vml_info("Ball xyz %f %f %f", balls[0].pos.x, balls[0].pos.y, balls[0].pos.z); */

    if (is_in_teleport_zone) {
        is_in_teleport_zone = balls[0].pos.z < -56.0f;
    }

    if (balls[0].pos.z < -56.0f && balls[0].pos.y < 10.0f) {
        balls[0].pos.y += 140.0f;
        balls[0].prev_pos.y += 140.0f;
        balls[0].g_some_pos1.y += 140.0f;
        cameras[0].pos.y += 140.0f;
        cameras[0].g_some_vec1.y += 140.0f;
        cameras[0].g_some_vec2.y += 140.0f;
        balls[0].y_pos_copy = 0.0f;

        is_in_teleport_zone = TRUE;
    } else if (!is_in_teleport_zone && balls[0].pos.z < -56.0f && balls[0].pos.y < 150.0f && balls[0].pos.y > 130.0f) {
        balls[0].pos.y += 40.0f;
        balls[0].prev_pos.y += 40.0f;
        balls[0].g_some_pos1.y += 40.0f;
        cameras[0].pos.y += 40.0f;
        cameras[0].g_some_vec1.y += 40.0f;
        cameras[0].g_some_vec2.y += 40.0f;
        balls[0].y_pos_copy = 0.0f;
    }
}

void _prolog(void) {
    vml_info("ENTER THE VOID");

    u32 fb_size = GXGetTexBufferSize(640, 448, GX_TF_CMPR, GX_FALSE, 0);
    vml_info("Allocating %u bytes for framebuffer texture", fb_size);
    void_fb = vml_malloc(fb_size);
    memset(void_fb, 0, fb_size);

    /* vml_patch_buf_inject_func(vml_main_loop_buffer_info->rel_buffer, 0x000056C0, &do_thing); */
    /* vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x00000970, PPC_INSTR_NOP()); // Take a pause menu screenshot *every* frame */
    /* vml_patch_buf_inject_func(vml_main_loop_buffer_info->rel_buffer, 0x00000A0C, &do_thing); */
    vml_patch_buf_inject_func(vml_main_loop_buffer_info->rel_buffer, 0x00000A0C, &test);

    // This disables the camera :)
    /* vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x00021370, PPC_INSTR_BLR()); */

    VVec(char*) vec = VVec_new(char*);

    VVec_push(vec, "I needed some test strings");
    VVec_push(vec, "Hello");
    VVec_push(vec, "nya~");
    VVec_push(vec, "uwu");
    VVec_push(vec, "Did you know I'm a furry?");
    VVec_push(vec, "owo");
    VVec_push(vec, "stuff");
    VVec_push(vec, "things");
    VVec_push(vec, "other things");
    VVec_push(vec, "blep");
    VVec_push(vec, "tertert");
    VVec_push(vec, "iowurwoperi;kl");
    VVec_push(vec, "r9ewriouywie");
    VVec_push(vec, "rewpiuljkwemn,rwe");

    for (u32 i = 0; i < VVec_length(vec); i++) {
        vml_info("test %s", vec[i]);
    }

    VVec_remove(vec, 0);

    for (u32 i = 0; i < VVec_length(vec); i++) {
        vml_info("test2 %s", vec[i]);
    }

    VVec_free(vec);
}

void _epilog(void) {
    vml_fatal("VOID _epilog was called - this was not expected");
}

void _unresolved(void) {
    vml_fatal("An unresolved function was called in VOID");
}

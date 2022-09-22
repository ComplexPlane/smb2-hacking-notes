#include "void/init.h"
#include "void/font.h"
#include "void/draw.h"
#include "void/stage_info.h"
#include "vml/patch.h"
#include "vml/alloc.h"
#include "vml/log.h"
#include "mkb2/gamecube.h"
#include "mkb2/ball.h"
#include "mkb2/camera.h"
#include "mkb2/display.h"
#include "mkb2/rel.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dolphin/gx/GXEnum.h"
#include "dolphin/gx/GXGeometry.h"

#include "v/container/vec.h"

extern u8 g_some_texture_data;
void *void_fb;

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

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    // Black quad
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

    // Game screen
    GXPosition3f32(-0.7f, -0.7f, 0.0f);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(0.0f, 0.0f);

    GXPosition3f32(-0.7f, 0.7f, 0.0f);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(0.0f, 1.0f);

    GXPosition3f32(0.7f, 0.7f, 0.0f);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(1.0f, 1.0f);

    GXPosition3f32(0.7f, -0.7f, 0.0f);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(1.0f, 0.0f);
    GXEnd();

    GXInitTexObj(&tex, void_fb, 640, 448, GX_TF_CMPR, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXLoadTexObj(&tex, GX_TEXMAP0);

    /* for (int i = 0; i < 1; i++) { */
        /* float scale = 1.03f; */
        /* [> u8 alpha = 20 * (1.0f - (i / 50.0f)); <] */
        /* [> u8 alpha = 240; <] */
        /* float rot = 0.00f; */

        /* MTXIdentity(view); */

        /* // I couldn't find MTXRotRad/MTXRotDeg, so we just create the rotation matrix manually here */
        /* Mtx44 rot_mtx = { */
            /* { cos(rot), -sin(rot), 0.0f, 0.0f }, */
            /* { sin(rot), cos(rot), 0.0f, 0.0f }, */
            /* { 0.0f, 0.0f, 1.0f, 0.0f }, */
            /* { 0.0f, 0.0f, 0.0f, 1.0f }, */
        /* }; */

        /* MTXConcat(view, rot_mtx, view); */
        /* GXLoadPosMtxImm(view, GX_PNMTX0); */

        /* GXBegin(GX_QUADS, GX_VTXFMT0, 8); */
        /* GXPosition3f32(-1.0f, 0.0f, 0.0f); */
        /* GXColor4u8(255, 255, 255, 255); */
        /* GXTexCoord2f32(0.0f, 0.5f); */

        /* GXPosition3f32(-1.0f, scale, 0.0f); */
        /* GXColor4u8(255, 255, 255, 240); */
        /* GXTexCoord2f32(0.0f, 1.0f); */

        /* GXPosition3f32(1.0f, scale, 0.0f); */
        /* GXColor4u8(255, 255, 255, 240); */
        /* GXTexCoord2f32(1.0f, 1.0f); */

        /* GXPosition3f32(1.0f, 0.0f, 0.0f); */
        /* GXColor4u8(255, 255, 255, 255); */
        /* GXTexCoord2f32(1.0f, 0.5f); */

        /* GXPosition3f32(-1.0f, -scale, 0.0f); */
        /* GXColor4u8(255, 255, 255, 240); */
        /* GXTexCoord2f32(0.0f, 0.0f); */

        /* GXPosition3f32(-1.0f, 0.0f, 0.0f); */
        /* GXColor4u8(255, 255, 255, 255); */
        /* GXTexCoord2f32(0.0f, 0.5f); */

        /* GXPosition3f32(1.0f, 0.0f, 0.0f); */
        /* GXColor4u8(255, 255, 255, 255); */
        /* GXTexCoord2f32(1.0f, 0.5f); */

        /* GXPosition3f32(1.0f, -scale, 0.0f); */
        /* GXColor4u8(255, 255, 255, 240); */
        /* GXTexCoord2f32(1.0f, 0.0f); */
        /* GXEnd(); */
    /* } */

    /* void_draw_pop_vtx_attr(GX_VTXFMT0, GX_VA_TEX0); */
    /* void_draw_pop_vtx_attr(GX_VTXFMT0, GX_VA_CLR0); */
    /* void_draw_pop_vtx_attr(GX_VTXFMT0, GX_VA_POS); */

    /* void_draw_pop_vtx_desc(); */

    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);

    /* GXSetTexCopySrc(0, 0, 640, 448); */
    /* GXSetTexCopyDst(640, 448, GX_TF_CMPR, GX_FALSE); */
    /* GXCopyTex(void_fb, GX_FALSE); */
}

static void do_thing2(void) {
    /* vml_info("GI %u", graphics_info->frame_count); */

    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTexGens(1);

    GXTexObj tex;
    /* GXInitTexObj(&tex, &g_some_texture_data, 640, 448, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE); */
    GXInitTexObj(&tex, void_fb, 640, 448, GX_TF_CMPR, GX_CLAMP, GX_CLAMP, GX_FALSE);
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

    /* GXClearVtxDesc(); */

    void_draw_push_vtx_desc();

    /* void_draw_push_vtx_attr(GX_VTXFMT0, GX_VA_POS); */
    /* void_draw_push_vtx_attr(GX_VTXFMT0, GX_VA_CLR0); */
    /* void_draw_push_vtx_attr(GX_VTXFMT0, GX_VA_TEX0); */

    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    Mtx44 view;

    GXBegin(GX_QUADS, GX_VTXFMT7, 8);
    // Black quad
    GXPosition3f32(-1.0f, -1.0f, 0.0f);
    GXColor4u8(0, 0, 0, 255);
    GXTexCoord2f32(0.0f, 0.0f);

    GXPosition3f32(-1.0f, 1.0f, 0.0f);
    GXColor4u8(0, 0, 0, 255);
    GXTexCoord2f32(0.0f, 1.0f);

    GXPosition3f32(1.0f, 1.0f, 0.0f);
    GXColor4u8(0, 0, 0, 255);
    GXTexCoord2f32(1.0f, 1.0f);

    GXPosition3f32(1.0f, -1.0f, 0.0f);
    GXColor4u8(0, 0, 0, 255);
    GXTexCoord2f32(1.0f, 0.0f);

    // Game screen
    GXPosition3f32(-1.0f, -1.0f, 0.0f);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(0.0f, 0.0f);

    GXPosition3f32(-1.0f, 1.0f, 0.0f);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(0.0f, 1.0f);

    GXPosition3f32(1.0f, 1.0f, 0.0f);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(1.0f, 1.0f);

    GXPosition3f32(1.0f, -1.0f, 0.0f);
    GXColor4u8(255, 255, 255, 255);
    GXTexCoord2f32(1.0f, 0.0f);
    GXEnd();

    GXInitTexObj(&tex, void_fb, 640, 448, GX_TF_CMPR, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXLoadTexObj(&tex, GX_TEXMAP0);

    /* for (int i = 0; i < 1; i++) { */
        /* float scale = 1.03f; */
        /* [> u8 alpha = 20 * (1.0f - (i / 50.0f)); <] */
        /* [> u8 alpha = 240; <] */
        /* float rot = 0.00f; */

        /* MTXIdentity(view); */

        /* // I couldn't find MTXRotRad/MTXRotDeg, so we just create the rotation matrix manually here */
        /* Mtx44 rot_mtx = { */
            /* { cos(rot), -sin(rot), 0.0f, 0.0f }, */
            /* { sin(rot), cos(rot), 0.0f, 0.0f }, */
            /* { 0.0f, 0.0f, 1.0f, 0.0f }, */
            /* { 0.0f, 0.0f, 0.0f, 1.0f }, */
        /* }; */

        /* MTXConcat(view, rot_mtx, view); */
        /* GXLoadPosMtxImm(view, GX_PNMTX0); */

        /* GXBegin(GX_QUADS, GX_VTXFMT0, 8); */
        /* GXPosition3f32(-1.0f, 0.0f, 0.0f); */
        /* GXColor4u8(255, 255, 255, 255); */
        /* GXTexCoord2f32(0.0f, 0.5f); */

        /* GXPosition3f32(-1.0f, scale, 0.0f); */
        /* GXColor4u8(255, 255, 255, 240); */
        /* GXTexCoord2f32(0.0f, 1.0f); */

        /* GXPosition3f32(1.0f, scale, 0.0f); */
        /* GXColor4u8(255, 255, 255, 240); */
        /* GXTexCoord2f32(1.0f, 1.0f); */

        /* GXPosition3f32(1.0f, 0.0f, 0.0f); */
        /* GXColor4u8(255, 255, 255, 255); */
        /* GXTexCoord2f32(1.0f, 0.5f); */

        /* GXPosition3f32(-1.0f, -scale, 0.0f); */
        /* GXColor4u8(255, 255, 255, 240); */
        /* GXTexCoord2f32(0.0f, 0.0f); */

        /* GXPosition3f32(-1.0f, 0.0f, 0.0f); */
        /* GXColor4u8(255, 255, 255, 255); */
        /* GXTexCoord2f32(0.0f, 0.5f); */

        /* GXPosition3f32(1.0f, 0.0f, 0.0f); */
        /* GXColor4u8(255, 255, 255, 255); */
        /* GXTexCoord2f32(1.0f, 0.5f); */

        /* GXPosition3f32(1.0f, -scale, 0.0f); */
        /* GXColor4u8(255, 255, 255, 240); */
        /* GXTexCoord2f32(1.0f, 0.0f); */
        /* GXEnd(); */
    /* } */

    /* void_draw_pop_vtx_attr(GX_VTXFMT7, GX_VA_TEX0); */
    /* void_draw_pop_vtx_attr(GX_VTXFMT7, GX_VA_CLR0); */
    /* void_draw_pop_vtx_attr(GX_VTXFMT7, GX_VA_POS); */

    void_draw_pop_vtx_desc();

    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);

    MTXPerspective(
            proj,
            60.0f, // FOV
            1.777777777f, // 16:9 aspect ratio
            0.1f, // Near clip
            20000.0f // Far clip
            );
    GXSetProjection(proj, GX_PERSPECTIVE);
}

static void teleport_ball_to(Vec3f pos) {
    balls[0].pos.x = pos.x;
    balls[0].pos.y = pos.y;
    balls[0].pos.z = pos.z;
}

extern u32 sub_mode;
extern u32 sub_mode_request;
extern u32 ball_mode;
extern u32 g_some_other_flags;
extern s32 sub_mode_frame_counter;
extern u32 g_some_bitflag2;
extern u32 Gameplay_Flag;
extern u32 monkey_flags;
/* static void fuck(void) { */
    /* if (sub_mode == 0x3A) { */
        /* [> sub_mode_request = 0x54; // GAME OVER <] */
        /* sub_mode_request = 0x33; */
        /* Vec3f target = {0.0f, 4.0f, 0.0f}; */
        /* teleport_ball_to(target); */
        /* vml_info("fuckign mode change"); */

        /* ball_mode &= ~0x4; */
        /* ball_mode &= ~0x8; */
        /* [> g_some_other_flags &= ~10; <] */
    /* } */
/* } */

extern s32 g_current_stage_id;

static void test(void) {
    /* vml_info("Ball xyz %f %f %f", balls[0].pos.x, balls[0].pos.y, balls[0].pos.z); */
    /* vml_info("BALL MODE %d", ball_mode & 0x10); */
    /* ball_mode |= 0x4; */

    /* vml_info("STGID %d", g_current_stage_id); */

    if (g_current_stage_id != -1) {
        void_stage_infos[g_current_stage_id]->on_tick();
    }

    /* fuck(); */
}

static void on_main_game_rel_loaded(RelBufferInfo *rel) {
    // This disables fallouts
    /* vml_patch_buf_inject_func(vml_main_loop_buffer_info->rel_buffer, 0x00002F64, &dbg); */
    vml_patch_instruction(rel->rel_buffer, 0x00002F64, PPC_INSTR_BLR());
    /* vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x00003040, PPC_INSTR_BLR()); */
    /* vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x000be7c0, PPC_INSTR_BLR()); */

    vml_patch_instruction(rel->rel_buffer, 0x000012ec, PPC_INSTR_NOP());
    vml_patch_instruction(rel->rel_buffer, 0x000012f4, PPC_INSTR_NOP());
    vml_patch_instruction(rel->rel_buffer, 0x000012f8, PPC_INSTR_NOP());
    vml_patch_instruction(rel->rel_buffer, 0x000012fc, PPC_INSTR_NOP());
    /* vml_patch_instruction(rel->rel_buffer, 0x0000130c, PPC_INSTR_NOP()); */

    /* vml_patch_buf_inject_func(rel->rel_buffer, 0x00002F64, &fuck); */
}

static void on_additional_rel_loaded(void) {
    RelBufferInfo *loaded_rel = g_loaded_rel_infos[g_loaded_rel_count - 1];
    if (*((u32*) loaded_rel->rel_buffer) == MKB2_MODULE_ID_MAIN_GAME) {
        on_main_game_rel_loaded(loaded_rel);
    }
}

static void capture(void) {
    test();

    GXSetTexCopySrc(0, 0, 640, 448);
    GXSetTexCopyDst(640, 448, GX_TF_CMPR, GX_FALSE);
    GXCopyTex(void_fb, GX_FALSE);
}

/* void g_something_with_backgrounds(void); */
/* static void maybe_draw_bg(void) { */
    /* g_something_with_backgrounds(); */
/* } */

static void void_patch_title_screen(void) {
    // Replace the slash in "PRESS START/PAUSE" with a null terminator for a cleaner title screen
    *((u8*) vml_main_loop_buffer_info->rel_buffer + 0x0020F3EC + 11) = 0;

    // Set the PRESS color to blue
    *((u32*) ((u8*) vml_main_loop_buffer_info->rel_buffer + 0x00177950)) = 0x0C5BE6FF;

    // Set the START color to blue
    *((u32*) ((u8*) vml_main_loop_buffer_info->rel_buffer + 0x00177950 + 4)) = 0x0C5BE6FF;

    // Set the "PRESS START" font to FONT_ASC_12x12
    vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x000bac6c, PPC_INSTR_LI(PPC_R0, 0x2));

    // Only make the text glow blue (nop out the red/green glow instructions)
    vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x000BADEC, PPC_INSTR_NOP());
    vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x000BADF4, PPC_INSTR_NOP());
}

void _prolog(void) {
    vml_info("ENTER THE VOID");

    u32 fb_size = GXGetTexBufferSize(640, 448, GX_TF_CMPR, GX_FALSE, 0);
    vml_info("Allocating %u bytes for framebuffer texture", fb_size);
    void_fb = vml_malloc(fb_size);
    memset(void_fb, 0, fb_size);

    /* vml_patch_buf_inject_func(vml_main_loop_buffer_info->rel_buffer, 0x000056C0, &do_thing); */
    vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x00000970, PPC_INSTR_NOP()); // Take a pause menu screenshot *every* frame
    /* vml_patch_buf_inject_func(vml_main_loop_buffer_info->rel_buffer, 0x00000A0C, &do_thing); */
    vml_patch_buf_inject_func(vml_main_loop_buffer_info->rel_buffer, 0x00006874, &do_thing2);
    vml_patch_buf_inject_func(vml_main_loop_buffer_info->rel_buffer, 0x00000A0C, &capture);
    /* vml_patch_buf_inject_func(vml_main_loop_buffer_info->rel_buffer, 0x00000A0C, &test); */

    // This stops backgrounds from drawing
    /* vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x000068EC, PPC_INSTR_NOP()); */
    /* vml_patch_buf_inject_func(vml_main_loop_buffer_info->rel_buffer, 0x000068EC, &maybe_draw_bg); */

    // This disables the camera :)
    /* vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x00021370, PPC_INSTR_BLR()); */

    /* vml_patch_buf_inject_func(vml_main_loop_buffer_info->rel_buffer, 0x00000248, &on_additional_rel_loaded); */

    // Disable ape drawing
    /* vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x001107E8, PPC_INSTR_BLR()); */

    // Disable background drawing
    /* vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x0006FAF0, PPC_INSTR_BLR()); */

    void_patch_title_screen();

    /* VVec(char*) vec = VVec_new(char*); */

    /* VVec_push(vec, "I needed some test strings"); */
    /* VVec_push(vec, "Hello"); */
    /* VVec_push(vec, "nya~"); */
    /* VVec_push(vec, "uwu"); */
    /* VVec_push(vec, "Did you know I'm a furry?"); */
    /* VVec_push(vec, "owo"); */
    /* VVec_push(vec, "stuff"); */
    /* VVec_push(vec, "things"); */
    /* VVec_push(vec, "other things"); */
    /* VVec_push(vec, "blep"); */
    /* VVec_push(vec, "tertert"); */
    /* VVec_push(vec, "iowurwoperi;kl"); */
    /* VVec_push(vec, "r9ewriouywie"); */
    /* VVec_push(vec, "rewpiuljkwemn,rwe"); */

    /* for (u32 i = 0; i < VVec_length(vec); i++) { */
        /* vml_info("test %s", vec[i]); */
    /* } */

    /* VVec_remove(vec, 0); */

    /* for (u32 i = 0; i < VVec_length(vec); i++) { */
        /* vml_info("test2 %s", vec[i]); */
    /* } */

    /* VVec_free(vec); */
}

void _epilog(void) {
    vml_fatal("VOID _epilog was called - this was not expected");
}

void _unresolved(void) {
    vml_fatal("An unresolved function was called in VOID");
}

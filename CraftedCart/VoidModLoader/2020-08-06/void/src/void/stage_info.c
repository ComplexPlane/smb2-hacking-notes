#include "void/stage_info.h"
#include <stdlib.h>

static void no_op(void) {}

static VoidStageInfo null_stage_info = {
    .on_tick = &no_op,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "mkb2/ball.h"
#include "mkb2/camera.h"

static BOOL is_in_teleport_zone = FALSE;
static void test_on_tick(void) {
    /* if (is_in_teleport_zone) { */
        /* is_in_teleport_zone = balls[0].pos.z < -56.0f; */
    /* } */

    /* if (balls[0].pos.z < -56.0f && balls[0].pos.y < 10.0f) { */
        /* balls[0].pos.y += 140.0f; */
        /* balls[0].prev_pos.y += 140.0f; */
        /* balls[0].g_some_pos1.y += 140.0f; */
        /* cameras[0].pos.y += 140.0f; */
        /* cameras[0].g_some_vec1.y += 140.0f; */
        /* cameras[0].g_some_vec2.y += 140.0f; */
        /* balls[0].y_pos_copy = 0.0f; */

        /* is_in_teleport_zone = TRUE; */
    /* } else if (!is_in_teleport_zone && balls[0].pos.z < -56.0f && balls[0].pos.y < 150.0f && balls[0].pos.y > 130.0f) { */
        /* balls[0].pos.y += 40.0f; */
        /* balls[0].prev_pos.y += 40.0f; */
        /* balls[0].g_some_pos1.y += 40.0f; */
        /* cameras[0].pos.y += 40.0f; */
        /* cameras[0].g_some_vec1.y += 40.0f; */
        /* cameras[0].g_some_vec2.y += 40.0f; */
        /* balls[0].y_pos_copy = 0.0f; */
    /* } */
}

static VoidStageInfo test_stage_info = {
    .on_tick = &test_on_tick,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "vml/patch.h"
#include "vml/log.h"

extern void *void_fb;

static void draw_glitchy(void) {
    float x_min = ((float) rand()) / SMB_RAND_MAX;
    float x_max = x_min + ((((float) rand()) / SMB_RAND_MAX) * 0.12f);
    float y_min = ((float) rand()) / SMB_RAND_MAX;
    float y_max = y_min + ((((float) rand()) / SMB_RAND_MAX) * 0.04f);

    float x_offset = (((float) rand()) / SMB_RAND_MAX) * 0.2f - 0.1f;
    float y_offset = (((float) rand()) / SMB_RAND_MAX) * 0.2f - 0.1f;

    u32 red = (((float) rand()) / SMB_RAND_MAX) * 255.0f;
    u32 green = (((float) rand()) / SMB_RAND_MAX) * 255.0f;
    u32 blue = (((float) rand()) / SMB_RAND_MAX) * 255.0f;

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

    /* void_draw_push_vtx_attr(GX_VTXFMT0, GX_VA_POS); */
    /* void_draw_push_vtx_attr(GX_VTXFMT0, GX_VA_CLR0); */
    /* void_draw_push_vtx_attr(GX_VTXFMT0, GX_VA_TEX0); */

    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT7, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    GXBegin(GX_QUADS, GX_VTXFMT7, 4);
    // Game screen
    GXPosition3f32(x_min * 2.0f - 1.0f + x_offset, y_min * 2.0f - 1.0 + y_offset, 0.0f);
    GXColor4u8(red, green, blue, 255);
    GXTexCoord2f32(x_min, y_min);

    GXPosition3f32(x_min * 2.0f - 1.0f + x_offset, y_max * 2.0f - 1.0 + y_offset, 0.0f);
    GXColor4u8(red, green, blue, 255);
    GXTexCoord2f32(x_min, y_max);

    GXPosition3f32(x_max * 2.0f - 1.0f + x_offset, y_max * 2.0f - 1.0 + y_offset, 0.0f);
    GXColor4u8(red, green, blue, 255);
    GXTexCoord2f32(x_max, y_max);

    GXPosition3f32(x_max * 2.0f - 1.0f + x_offset, y_min * 2.0f - 1.0 + y_offset, 0.0f);
    GXColor4u8(red, green, blue, 255);
    GXTexCoord2f32(x_max, y_min);
    GXEnd();

    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_ENABLE);
}

static void glitch_fx_tick(void) {
    float threshold = ((balls[0].pos.z + 56.0f) / 74.0f) * -1.0f * 0.03f;
    if (rand() < SMB_RAND_MAX * threshold) {
        for (u32 i = 0; i < 30; i++) {
            draw_glitchy();
        }
    }
}

extern u32 dip_switches;

static void banks_on_tick(void) {
    vml_info("FUCK");

    if (balls[0].pos.z < -56.0f) {
        vml_patch_instruction(vml_main_loop_buffer_info->rel_buffer, 0x000068ec, PPC_INSTR_NOP());
        ICInvalidateRange((u8*) vml_main_loop_buffer_info->rel_buffer + 0x000068ec, 4);
        DCFlushRange((u8*) vml_main_loop_buffer_info->rel_buffer + 0x000068ec, 4);

        glitch_fx_tick();

        // Freeze the camera beyond -120!! :3
        if (balls[0].pos.z < -120.0f) {
            cameras[0].g_some_mode = 65;
            /* dip_switches = dip_switches | 128; // DIP_FIX_WORLD */
        }
    }
}

static VoidStageInfo banks_stage_info = {
    .on_tick = &banks_on_tick,
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VoidStageInfo *void_stage_infos[421] = {
    &null_stage_info,
    &test_stage_info, // st001
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &banks_stage_info, // st221 - Banks
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
    &null_stage_info,
};

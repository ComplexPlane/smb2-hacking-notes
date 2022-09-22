#include "vml/config/init.h"
#include "vml/config/font.h"
#include "vml/log.h"
#include "mkb2/gamecube.h"
#include "mkb2/display.h"
#include <stdlib.h>

#include "dolphin/gx/GXEnum.h"
#include "dolphin/gx/GXGeometry.h"

void _prolog(void) {
    vml_info("ENTER THE VOID");
    vml_info("(Loading VOID main loop)");

    srand(1337);

    static const GXColor clear_col = {4, 4, 4, 255};
    GXSetCopyClear(clear_col, GX_MAX_Z24);

    GXSetViewport(
            0.0f,
            0.0f,
            (float) current_render_mode->fbWidth,
            (float) current_render_mode->efbHeight,
            0.0f,
            1.0f
            );

    Mtx44 proj;
    /* MTXOrtho( */
    /*         proj, */
    /*         0.0f, */
    /*         480.0f, */
    /*         0.0f, */
    /*         640.0f, */
    /*         0.0f, */
    /*         20000.0f */
    /*         ); */
    /* MTXOrtho(proj, 0.0f, 1080.0f, 0.0f, 1920.0f, 0.0f, 20000.0f); */
    MTXPerspective(
            proj,
            60.0f, // FOV
            1.777777777f, // 16:9 aspect ratio
            0.1f, // Near clip
            20000.0f // Far clip
            );
    GXSetProjection(proj, GX_PERSPECTIVE);

    vml_init_fonts();

    GXSetCullMode(GX_CULL_BACK);

    GXSetNumChans(1);

    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTexGens(1);
    GXLoadTexObj(&vml_font_exo2_tex, GX_TEXMAP0);

    GXSetNumTevStages(1);
    /* GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR1A1); */
    /* GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC); */
    /* GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV); */
    /* GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA); */
    /* GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV); */

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR1A1);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_ENABLE, GX_TEVPREV);

    /* GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE); */
    /* GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0); */

    /* GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE); */
    /* GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL); */
    // set texture object
    /* GXLoadTexObj(&BallTexObj, GX_TEXMAP0); */
    // enable alpha blending
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);

    while (TRUE) {
        gx_start_new_frame();

        Mtx44 view;
        Vec cam = {0.0f, 0.0f, 20.0f};
        static Vec target = {0.0f, 0.0f, -99999.0f};
        static Vec up = {0.0f, 1.0f, 0.0f};
        MTXLookAt(view, &cam, &up, &target);

        GXLoadPosMtxImm(view, GX_PNMTX0);

        /* GXInvalidateVtxCache(); */

        GXClearVtxDesc();

        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

        GXBegin(GX_QUADS, GX_VTXFMT0, 4 * ((2000 / 20) * (1000 / 20)));
        for (f32 x = -1000.0f; x < 1000.0f; x += 20.0f) {
            for (f32 z = -800.0f; z < 200.0f; z += 20.0f) {
                f32 y = 50.0f;
                /* u8 alpha = ((z - 200.0f) / 1000) * 255; */

                GXPosition3f32(x - 1.5f, y - 1.5f, z);
                GXColor4u8(255, 255, 0, 0);
                GXTexCoord2f32(0.0f, 1.0f);

                GXPosition3f32(x - 1.5f, y + 1.5f, z);
                GXColor4u8(255, 0, 255, 255);
                GXTexCoord2f32(0.0f, 0.0f);

                GXPosition3f32(x + 1.5f, y + 1.5f, z);
                GXColor4u8(0, 255, 255, 255);
                GXTexCoord2f32(1.0f, 0.0f);

                GXPosition3f32(x + 1.5f, y - 1.5f, z);
                GXColor4u8(255, 0, 255, 255);
                GXTexCoord2f32(1.0f, 1.0f);
            }
        }
        GXEnd();

        vml_draw_text("Hello, world!", 0.0f, 0.0f);

        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        GXSetColorUpdate(GX_TRUE);
        GXCopyDisp(graphics_info->active_framebuffer, GX_TRUE);

        video_finish_frame();

        PADStatus pad_statuses[PAD_MAX_CONTROLLERS];
        PADRead(pad_statuses);

        // Press B to exit
        if (pad_statuses[0].err == PAD_ERR_NONE && pad_statuses[0].button & PAD_BUTTON_B) {
            break;
        }
    }
}

void _epilog(void) {
    vml_info("Void Mod Loader config unloaded");
}

void _unresolved(void) {
    vml_fatal("An unresolved function was called in Void Mod Loader config");
}

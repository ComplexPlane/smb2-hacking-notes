#include "void/font.h"
#include "vml/log.h"
#include <string.h>

static VmlCharacter vml_font_exo2_characters[] = {
    {' ', 509, 57, 3, 3, 1, 1},
    {'!', 44, 57, 5, 24, -2, 23},
    {'"', 434, 57, 10, 9, -1, 23},
    {'#', 363, 0, 22, 24, 0, 23},
    {'$', 28, 0, 18, 32, 0, 27},
    {'%', 284, 0, 29, 24, 0, 23},
    {'&', 339, 0, 24, 24, -1, 23},
    {'\'', 444, 57, 5, 9, -1, 23},
    {'(', 0, 0, 12, 33, 0, 26},
    {')', 12, 0, 11, 33, 0, 26},
    {'*', 383, 57, 14, 13, 0, 23},
    {'+', 328, 57, 17, 16, 0, 18},
    {',', 414, 57, 6, 11, -1, 5},
    {'-', 496, 57, 13, 5, -1, 11},
    {'.', 474, 57, 6, 6, -1, 5},
    {'/', 114, 0, 18, 27, 1, 23},
    {'0', 174, 33, 18, 24, -1, 23},
    {'1', 0, 57, 12, 24, 1, 23},
    {'2', 192, 33, 18, 24, 0, 23},
    {'3', 318, 33, 17, 24, 0, 23},
    {'4', 470, 0, 20, 24, 0, 23},
    {'5', 335, 33, 17, 24, 0, 23},
    {'6', 352, 33, 17, 24, -1, 23},
    {'7', 369, 33, 17, 24, 0, 23},
    {'8', 210, 33, 18, 24, -1, 23},
    {'9', 228, 33, 18, 24, 0, 23},
    {':', 377, 57, 6, 16, -1, 15},
    {';', 84, 57, 6, 21, -1, 15},
    {'<', 345, 57, 16, 16, 0, 18},
    {'=', 397, 57, 17, 12, -1, 15},
    {'>', 361, 57, 16, 16, -1, 18},
    {'?', 237, 0, 16, 25, 0, 24},
    {'@', 49, 57, 21, 23, -1, 20},
    {'A', 385, 0, 22, 24, 1, 23},
    {'B', 246, 33, 18, 24, -1, 23},
    {'C', 60, 33, 19, 24, 0, 23},
    {'D', 490, 0, 20, 24, -1, 23},
    {'E', 386, 33, 17, 24, -1, 23},
    {'F', 403, 33, 17, 24, -1, 23},
    {'G', 79, 33, 19, 24, 0, 23},
    {'H', 98, 33, 19, 24, -1, 23},
    {'I', 32, 57, 6, 24, -1, 23},
    {'J', 12, 57, 10, 24, 0, 23},
    {'K', 117, 33, 19, 24, -1, 23},
    {'L', 471, 33, 16, 24, -1, 23},
    {'M', 313, 0, 26, 24, -1, 23},
    {'N', 0, 33, 20, 24, -1, 23},
    {'O', 407, 0, 21, 24, 0, 23},
    {'P', 264, 33, 18, 24, -1, 23},
    {'Q', 93, 0, 21, 30, 0, 23},
    {'R', 136, 33, 19, 24, -1, 23},
    {'S', 282, 33, 18, 24, 0, 23},
    {'T', 20, 33, 20, 24, 1, 23},
    {'U', 155, 33, 19, 24, -1, 23},
    {'V', 428, 0, 21, 24, 1, 23},
    {'W', 253, 0, 31, 24, 0, 23},
    {'X', 449, 0, 21, 24, 1, 23},
    {'Y', 40, 33, 20, 24, 1, 23},
    {'Z', 300, 33, 18, 24, 0, 23},
    {'[', 46, 0, 10, 32, -1, 26},
    {'\\', 132, 0, 18, 27, 1, 23},
    {']', 76, 0, 9, 32, 0, 26},
    {'^', 420, 57, 14, 10, 0, 20},
    {'_', 480, 57, 16, 5, 1, 1},
    {'`', 449, 57, 10, 8, 0, 24},
    {'a', 200, 57, 17, 18, 0, 17},
    {'b', 420, 33, 17, 24, -1, 23},
    {'c', 251, 57, 16, 18, 0, 17},
    {'d', 437, 33, 17, 24, 0, 23},
    {'e', 217, 57, 17, 18, 0, 17},
    {'f', 150, 0, 15, 26, 1, 25},
    {'g', 165, 0, 19, 25, 0, 17},
    {'h', 454, 33, 17, 24, -1, 23},
    {'i', 38, 57, 6, 24, -1, 23},
    {'j', 85, 0, 8, 31, 1, 23},
    {'k', 487, 33, 16, 24, -1, 23},
    {'l', 22, 57, 10, 24, -1, 23},
    {'m', 118, 57, 26, 18, -1, 17},
    {'n', 234, 57, 17, 18, -1, 17},
    {'o', 182, 57, 18, 18, 0, 17},
    {'p', 203, 0, 17, 25, -1, 17},
    {'q', 220, 0, 17, 25, 0, 17},
    {'r', 315, 57, 13, 18, -1, 17},
    {'s', 267, 57, 16, 18, 0, 17},
    {'t', 70, 57, 14, 23, 1, 22},
    {'u', 283, 57, 16, 18, -1, 17},
    {'v', 144, 57, 19, 18, 1, 17},
    {'w', 90, 57, 28, 18, 1, 17},
    {'x', 163, 57, 19, 18, 1, 17},
    {'y', 184, 0, 19, 25, 1, 17},
    {'z', 299, 57, 16, 18, 0, 17},
    {'{', 56, 0, 10, 32, 0, 26},
    {'|', 23, 0, 5, 33, -1, 26},
    {'}', 66, 0, 10, 32, 0, 26},
    {'~', 459, 57, 15, 7, -1, 13},
};

VmlFont vml_font_exo2 = {
    "Exo 2",
    32,
    0,
    0,
    512,
    128,
    95,
    vml_font_exo2_characters
};

GXTexObj vml_font_exo2_tex;

static inline u32 round_up_next_32(u32 value) {
    return value + 0x1F & 0xFFFFFFE0;
}

void vml_init_fonts(void) {
    static char *file_name = "/vml/font/exo2.tpl";

    // Try open the file
    DVDFileInfo file_info;
    if (!DVDOpen(file_name, &file_info)) {
        vml_fatal("Could not DVDOpen %s", file_name);
    }

    // Allocate memory to read in the file
    u32 tex_len_32b_aligned = round_up_next_32(file_info.length);
    void *buffer = OSAllocFromArenaLo(tex_len_32b_aligned, 32);
    if (buffer == NULL) {
        DVDClose(&file_info);
        vml_fatal("Could not allocate memory for %s", file_name);
    }

    // Read in the file
    s32 dvd_read_status = DVDRead(&file_info, buffer, tex_len_32b_aligned, 0);
    if (dvd_read_status < 0) {
        DVDClose(&file_info);
        vml_fatal("Failed to read %s with status code %d", file_name, dvd_read_status);
    }

    DVDClose(&file_info);

    GXInitTexObj(
            &vml_font_exo2_tex,
            ((u8*) buffer) + 0x20,
            vml_font_exo2.width,
            vml_font_exo2.height,
            GX_TF_IA4,
            GX_CLAMP,
            GX_CLAMP,
            GX_FALSE
            );

    GXInvalidateTexAll();

    vml_info("Loaded %s at 0x%08X", file_name, buffer);
}

static VmlCharacter* get_char_info(u32 code_point) {
    for (u32 i = 0; i < vml_font_exo2.character_count; i++) {
        VmlCharacter *c = vml_font_exo2.characters + i;
        if (c->code_point == code_point) {
            return c;
        }
    }

    // No matching char found
    return NULL;
}

void vml_draw_text(const char *text, float x, float y) {
    u32 len = strlen(text);

    GXClearVtxDesc();

    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);

    float char_x = x;
    float char_y = y;
    GXBegin(GX_QUADS, GX_VTXFMT0, 4 * len);
    for (u32 i = 0; i < len; i++) {
        VmlCharacter *c = get_char_info(text[i]);

        float x1 = char_x + c->origin_x;
        float y2 = char_y + c->origin_y;
        float x2 = x1 + c->width;
        float y1 = y2 - c->height;

        float uv_x1 = (float) c->x / vml_font_exo2.width;
        float uv_y1 = (float) c->y / vml_font_exo2.height;
        float uv_x2 = uv_x1 + ((float) c->width / vml_font_exo2.width);
        float uv_y2 = uv_y1 + ((float) c->height / vml_font_exo2.height);

        float z = 0.0f;

        GXPosition3f32(x1, y1, z);
        GXColor4u8(255, 255, 0, 0);
        GXTexCoord2f32(uv_x1, uv_y2);

        GXPosition3f32(x1, y2, z);
        GXColor4u8(255, 0, 255, 255);
        GXTexCoord2f32(uv_x1, uv_y1);

        GXPosition3f32(x2, y2, z);
        GXColor4u8(0, 255, 255, 255);
        GXTexCoord2f32(uv_x2, uv_y1);

        GXPosition3f32(x2, y1, z);
        GXColor4u8(255, 0, 255, 255);
        GXTexCoord2f32(uv_x2, uv_y2);

        char_x = x2;
    }
    GXEnd();
}

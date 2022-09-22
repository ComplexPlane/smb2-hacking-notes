#ifndef VML_VML_CONFIG_FONT_H
#define VML_VML_CONFIG_FONT_H

#include "mkb2/gamecube.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct VmlCharacter {
    u32 code_point;
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    s32 origin_x;
    s32 origin_y;
} VmlCharacter;

typedef struct VmlFont {
    const char *name;
    u32 size;
    u32 bold;
    u32 italic;
    u16 width;
    u16 height;
    u32 character_count;
    VmlCharacter *characters;
} VmlFont;

extern VmlFont vml_font_exo2;
extern GXTexObj vml_font_exo2_tex;

void vml_init_fonts(void);
void vml_draw_text(const char *text, float x, float y);

#ifdef __cplusplus
}
#endif

#endif

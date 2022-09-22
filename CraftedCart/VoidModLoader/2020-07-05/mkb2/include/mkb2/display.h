#ifndef VML_MKB2_MKB2_DISPLAY_H
#define VML_MKB2_MKB2_DISPLAY_H

#include "mkb2/gamecube.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GraphicsInfo {
    void *active_framebuffer;
    void *framebuffer1;
    void *framebuffer2;
    u32 frame_count;
    u32 active_framebuffer_index;
    GXFifoObj *fifos[2];
} GraphicsInfo;

extern GXRenderModeObj *current_render_mode;
extern GraphicsInfo *graphics_info;

void gp_wait(void);
void video_finish_frame(void);
void gx_start_new_frame(void);

#ifdef __cplusplus
}
#endif

#endif

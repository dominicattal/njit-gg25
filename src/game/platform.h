#ifndef PLATFORM_H
#define PLATFORM_H

#include "../util/util.h"

typedef enum {
    PLATFORM_1
} PlatformID;

typedef struct {
    vec2 position;
    vec2 size;
} Platform;

void platform_context_init(void);
void platform_context_update(f32 dt);
void platform_context_clear_platforms(void);
void platform_context_destroy(void);
void platform_context_prepare_render(void);
void platform_context_render(void);
const Array* platform_context_get_platforms(void);

Platform* platform_create(PlatformID id);
void platform_update(Platform* platform, f32 dt);
void platform_destroy(Platform* platform);
void platform_get_tex_info(Platform* platform, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2);

#endif
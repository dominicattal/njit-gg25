#include "platform.h"
#include "../renderer/renderer.h"
#include <stdlib.h>

typedef struct {
    Array* platforms;
    f32* vbo_buffer;
    u32* ebo_buffer;
    u32 vbo_length, ebo_length;
    u32 vbo_capacity, ebo_capacity;
    u32 platform_vao;
    u32 platform_vbo;
    u32 platform_ebo;
} PlatformContext;

static PlatformContext ctx;


#define NEW_PLATFORMS_PER_RESIZE 5
static void push_platform_into_buffer(Platform* platform)
{
    static i32 dx[] = {0, 0, 1, 1};
    static i32 dy[] = {0, 1, 1, 0};
    static u32 winding[] = { 0, 1, 2, 0, 2, 3 };
    if (ctx.vbo_capacity < ctx.vbo_length + 20) {
        ctx.vbo_capacity += NEW_PLATFORMS_PER_RESIZE * 20;
        ctx.ebo_capacity += NEW_PLATFORMS_PER_RESIZE * 6;
        if (ctx.vbo_buffer == NULL) {
            ctx.vbo_buffer = malloc(ctx.vbo_capacity * sizeof(f32));
            ctx.ebo_buffer = malloc(ctx.ebo_capacity * sizeof(u32));
        } else {
            ctx.vbo_buffer = realloc(ctx.vbo_buffer, ctx.vbo_capacity * sizeof(f32));
            ctx.ebo_buffer = realloc(ctx.ebo_buffer, ctx.ebo_capacity * sizeof(u32));
        }
    }
    f32 x1, x2, y1, y2;
    u32 tex;
    platform_get_tex_info(platform, &tex, &x1, &x2, &y1, &y2);
    for (i32 i = 0; i < 4; i++) {
        ctx.vbo_buffer[ctx.vbo_length++] = platform->position.x + dx[i] * platform->size.x;
        ctx.vbo_buffer[ctx.vbo_length++] = platform->position.y + dy[i] * platform->size.y;
        ctx.vbo_buffer[ctx.vbo_length++] = (dx[i]) ? x2 : x1;
        ctx.vbo_buffer[ctx.vbo_length++] = (dy[i]) ? y2 : y1;
        ctx.vbo_buffer[ctx.vbo_length++] = tex;
    }
    u32 idx = 4 * ctx.ebo_length / 6;
    for (i32 i = 0; i < 6; i++)
        ctx.ebo_buffer[ctx.ebo_length++] = winding[i] + idx;
}
void platform_context_init(void)
{
    glGenVertexArrays(1, &ctx.platform_vao);
    glGenBuffers(1, &ctx.platform_vbo);
    glGenBuffers(1, &ctx.platform_ebo);
    glBindVertexArray(ctx.platform_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.platform_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(0 * sizeof(f32)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(2 * sizeof(f32)));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(4 * sizeof(f32)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    ctx.platforms = array_create();
}

void platform_context_update(f32 dt)
{
    ctx.vbo_length = 0;
    ctx.ebo_length = 0;
    for (i32 i = ctx.platforms->length - 1; i >= 0; i--) {
        Platform* platform = array_get(ctx.platforms, i);
        platform_update(platform, dt);
        push_platform_into_buffer(platform);
    }
}

void platform_context_destroy(void)
{
    while (!array_empty(ctx.platforms)) {
        Platform* platform = array_pop(ctx.platforms, 0);
        platform_destroy(platform);
    }
    free(ctx.vbo_buffer);
    free(ctx.ebo_buffer);
    glDeleteVertexArrays(1, &ctx.platform_vao);
    glDeleteBuffers(1, &ctx.platform_vbo);
    glDeleteBuffers(1, &ctx.platform_ebo);
}

void platform_context_prepare_render(void)
{
    glBindBuffer(GL_ARRAY_BUFFER, ctx.platform_vbo);
    glBufferData(GL_ARRAY_BUFFER, ctx.vbo_length * sizeof(f32), ctx.vbo_buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.platform_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ctx.ebo_length * sizeof(u32), ctx.ebo_buffer, GL_STATIC_DRAW); 
}

void platform_context_render(void)
{
    shader_use(SHADER_GAME);

    glBindVertexArray(ctx.platform_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.platform_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.platform_ebo);
    glDrawElements(GL_TRIANGLES, ctx.ebo_length, GL_UNSIGNED_INT, 0);
}

// --------------------------------

Platform* platform_create(PlatformID id)
{
    Platform* platform = malloc(sizeof(Platform));
    array_push(ctx.platforms, platform);
    return platform;
}

void platform_update(Platform* platform, f32 dt)
{

}

void platform_destroy(Platform* platform)
{
    free(platform);
}

void platform_get_tex_info(Platform* platform, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2)
{
    *x1 = 0;
    *x2 = 1;
    *y1 = 1;
    *y2 = 0;
    *tex = TEX_COLOR;
}
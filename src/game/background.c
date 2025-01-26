#include "background.h"
#include "../renderer/renderer.h"
#include "../window/window.h"
#include <stdlib.h>

static u32 background_tex_map[NUM_BACKGROUNDS] = {
    TEX_BACKGROUND_1,
    TEX_BUBBLES,
};

typedef struct {
    Background current_background_far;
    Background current_background_near;
    f32* vbo_buffer;
    u32* ebo_buffer;
    u32 vbo_length, ebo_length;
    u32 background_vao;
    u32 background_vbo;
    u32 background_ebo;
} BackgroundContext;

static BackgroundContext ctx;

void background_context_init(void)
{
    glGenVertexArrays(1, &ctx.background_vao);
    glGenBuffers(1, &ctx.background_vbo);
    glGenBuffers(1, &ctx.background_ebo);
    glBindVertexArray(ctx.background_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.background_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(0 * sizeof(f32)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(2 * sizeof(f32)));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(4 * sizeof(f32)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    ctx.vbo_length = 40;
    ctx.ebo_length = 12;
    ctx.vbo_buffer = malloc(ctx.vbo_length * sizeof(f32));
    ctx.ebo_buffer = malloc(ctx.ebo_length * sizeof(u32));

    ctx.current_background_far = BACKGROUND_1;
    ctx.current_background_near = BACKGROUND_2;
}

static void push_background_data(vec2 center, f32 zoom, Background background, u32 vbo_offset, u32 ebo_offset, i32 mulitplier)
{
    static i32 dx[] = {0, 0, 1, 1};
    static i32 dy[] = {0, 1, 1, 0};
    static u32 winding[] = { 0, 1, 2, 0, 2, 3 };

    for (i32 i = 0; i < 4; i++) {
        ctx.vbo_buffer[vbo_offset+5*i]   = 2 * dx[i] - 1;
        ctx.vbo_buffer[vbo_offset+5*i+1] = 2 * dy[i] - 1;
        ctx.vbo_buffer[vbo_offset+5*i+2] = dx[i];
        ctx.vbo_buffer[vbo_offset+5*i+3] = 1-dy[i];
        ctx.vbo_buffer[vbo_offset+5*i+4] = background_tex_map[background];
    }
    for (i32 i = 0; i < 6; i++)
        ctx.ebo_buffer[ebo_offset+i] = winding[i] + ebo_offset * 2 / 3;
}

void background_context_update(vec2 center, f32 zoom)
{
    push_background_data(center, zoom, ctx.current_background_far, 0, 0, 1.0);
    //push_background_data(center, zoom, ctx.current_background_near, 20, 6, 2.0);
}

void background_context_destroy(void)
{
    free(ctx.vbo_buffer);
    free(ctx.ebo_buffer);
    glDeleteVertexArrays(1, &ctx.background_vao);
    glDeleteBuffers(1, &ctx.background_vbo);
    glDeleteBuffers(1, &ctx.background_ebo);
}

void background_context_prepare_render(void)
{
    glBindBuffer(GL_ARRAY_BUFFER, ctx.background_vbo);
    glBufferData(GL_ARRAY_BUFFER, ctx.vbo_length * sizeof(f32), ctx.vbo_buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.background_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ctx.ebo_length * sizeof(u32), ctx.ebo_buffer, GL_STATIC_DRAW); 
}

void background_context_render(void)
{
    shader_use(SHADER_BACKGROUND);

    glBindVertexArray(ctx.background_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.background_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.background_ebo);
    glDrawElements(GL_TRIANGLES, ctx.ebo_length, GL_UNSIGNED_INT, 0);
}

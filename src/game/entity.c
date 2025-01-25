#include "entity.h"
#include "../renderer/renderer.h"
#include "../window/window.h"
#include "entities/entities.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    u32 id;
} Entity;

typedef struct {
    u32 entity_vao;
    u32 entity_vbo;
    u32 entity_ebo;
    f32* vbo_buffer;
    u32* ebo_buffer;
} EntityContext;

static EntityContext ctx;

void entity_init(void)
{
    glGenVertexArrays(1, &ctx.entity_vao);
    glGenBuffers(1, &ctx.entity_vbo);
    glGenBuffers(1, &ctx.entity_ebo);
    glBindVertexArray(ctx.entity_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.entity_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(0 * sizeof(f32)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(2 * sizeof(f32)));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(4 * sizeof(f32)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    ctx.vbo_buffer = malloc(8 * sizeof(f32));
    ctx.ebo_buffer = malloc(6 * sizeof(u32));
    f32 data_vbo[] = {
        0.0f, 0.0f, pubbles_frames[0].x1, pubbles_frames[0].y2, TEX_PUBBLES,
        0.0f, 1.0f, pubbles_frames[0].x1, pubbles_frames[0].y1, TEX_PUBBLES,
        1.0f, 1.0f, pubbles_frames[0].x2, pubbles_frames[0].y1, TEX_PUBBLES,
        1.0f, 0.0f, pubbles_frames[0].x2, pubbles_frames[0].y2, TEX_PUBBLES
    };
    u32 data_ebo[] = { 0, 1, 2, 0, 2, 3 };

    glBindBuffer(GL_ARRAY_BUFFER, ctx.entity_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data_vbo), data_vbo, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.entity_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data_ebo), data_ebo, GL_STATIC_DRAW);
}

void entity_destroy(void)
{
    free(ctx.vbo_buffer);
    free(ctx.ebo_buffer);
    glDeleteVertexArrays(1, &ctx.entity_vao);
    glDeleteBuffers(1, &ctx.entity_vbo);
    glDeleteBuffers(1, &ctx.entity_ebo);
}

void entity_prepare_render(void)
{

}

void entity_render(void)
{
    shader_use(SHADER_GAME);

    glBindVertexArray(ctx.entity_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.entity_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.entity_ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

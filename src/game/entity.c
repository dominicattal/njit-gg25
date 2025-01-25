#include "entity.h"
#include "../renderer/renderer.h"
#include "../window/window.h"
#include <stdio.h>

typedef struct {
    u32 id;
} Entity;

typedef struct {
    u32 entity_vao;
    u32 entity_vbo;
    u32 entity_ebo;
    f32* mesh_buffer;
} EntityContext;

static EntityContext ctx;

void entity_init(void)
{
    glGenVertexArrays(1, &ctx.entity_vao);
    glGenBuffers(1, &ctx.entity_vbo);
    glGenBuffers(1, &ctx.entity_ebo);
    glBindVertexArray(ctx.entity_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.entity_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)(0 * sizeof(f32)));
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    static f32 data_vbo[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    static u32 data_ebo[] = { 0, 1, 2, 0, 2, 3 };
    glBindBuffer(GL_ARRAY_BUFFER, ctx.entity_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data_vbo), data_vbo, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.entity_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(data_ebo), data_ebo, GL_STATIC_DRAW);
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

void entity_destroy(void)
{
    glDeleteVertexArrays(1, &ctx.entity_vao);
    glDeleteBuffers(1, &ctx.entity_vbo);
    glDeleteBuffers(1, &ctx.entity_ebo);
}
#include "entity.h"
#include "../renderer/renderer.h"
#include "../window/window.h"
#include "entities/entities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    Array* entities;
    u32 entity_vao;
    u32 entity_vbo;
    u32 entity_ebo;
    f32* vbo_buffer;
    u32* ebo_buffer;
    u32 vbo_length, ebo_length;
    u32 vbo_capacity, ebo_capacity;
} EntityContext;

static EntityContext ctx;

#define NEW_ENTS_PER_RESIZE 5
static void push_entity_into_buffer(Entity* ent)
{
    static i32 dx[] = {0, 0, 1, 1};
    static i32 dy[] = {0, 1, 1, 0};
    static u32 winding[] = { 0, 1, 2, 0, 2, 3 };
    if (ctx.vbo_capacity < ctx.vbo_length + 20) {
        ctx.vbo_capacity = NEW_ENTS_PER_RESIZE * 20;
        ctx.ebo_capacity = NEW_ENTS_PER_RESIZE * 6;
        if (ctx.vbo_buffer == NULL) {
            ctx.vbo_buffer = malloc(ctx.vbo_capacity * sizeof(f32));
            ctx.ebo_buffer = malloc(ctx.ebo_capacity * sizeof(u32));
        } else {
            ctx.vbo_buffer = realloc(ctx.vbo_buffer, ctx.vbo_capacity * sizeof(f32));
            ctx.ebo_buffer = realloc(ctx.ebo_buffer, ctx.ebo_capacity * sizeof(u32));
        }
    }
    for (i32 i = 0; i < 4; i++) {
        ctx.vbo_buffer[ctx.vbo_length++] = dx[i];
        ctx.vbo_buffer[ctx.vbo_length++] = dy[i];
        ctx.vbo_buffer[ctx.vbo_length++] = pubbles_frames[ent->frame][2*dx[i]];
        ctx.vbo_buffer[ctx.vbo_length++] = pubbles_frames[ent->frame][2*dy[i]+1];
        ctx.vbo_buffer[ctx.vbo_length++] = TEX_PUBBLES;
    }
    u32 idx = 4 * ctx.ebo_length / 6;
    for (i32 i = 0; i < 6; i++)
        ctx.ebo_buffer[ctx.ebo_length++] = winding[i] + idx;
}

Entity* entity_create(EntityID id)
{
    Entity* ent = malloc(sizeof(Entity));
    array_push(ctx.entities, ent);
    ent->id = id;
    ent->frame = 0;
    ent->timer = 0;
    return ent;
}

void entity_update(Entity* entity, f32 dt)
{
    entity->timer -= dt;
    if (entity->timer < 0) {
        entity->timer += 0.04;
        entity->frame = (entity->frame + 1) % 10;
    }
}

void entity_destroy(Entity* entity)
{
    free(entity);
}

void entity_context_init(void)
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

    ctx.entities = array_create();
    ctx.vbo_length = 0;
    ctx.ebo_length = 0;
    ctx.vbo_capacity = 0;
    ctx.ebo_capacity = 0;
    ctx.vbo_buffer = NULL;
    ctx.ebo_buffer = NULL;

    entity_create(ENT_PUBBLES);
}

void entity_context_destroy(void)
{
    while (!array_empty(ctx.entities))
        entity_destroy((Entity*)array_pop(ctx.entities, 0));
    array_destroy(ctx.entities);
    free(ctx.vbo_buffer);
    free(ctx.ebo_buffer);
    glDeleteVertexArrays(1, &ctx.entity_vao);
    glDeleteBuffers(1, &ctx.entity_vbo);
    glDeleteBuffers(1, &ctx.entity_ebo);
}

void entity_context_update(f32 dt)
{
    ctx.vbo_length = 0;
    ctx.ebo_length = 0;
    for (i32 i = 0; i < ctx.entities->length; i++) {
        entity_update(array_get(ctx.entities, i), dt);
        push_entity_into_buffer(array_get(ctx.entities, i));
    }
}

void entity_context_prepare_render(void)
{
    glBindBuffer(GL_ARRAY_BUFFER, ctx.entity_vbo);
    glBufferData(GL_ARRAY_BUFFER, ctx.vbo_length * sizeof(f32), ctx.vbo_buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.entity_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ctx.ebo_length * sizeof(u32), ctx.ebo_buffer, GL_STATIC_DRAW);
}

void entity_context_render(void)
{
    shader_use(SHADER_GAME);

    glBindVertexArray(ctx.entity_vao);
    glBindBuffer(GL_ARRAY_BUFFER, ctx.entity_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.entity_ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

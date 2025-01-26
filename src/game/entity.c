#include "entity.h"
#include "../renderer/renderer.h"
#include "../window/window.h"
#include "entities/entities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ENTITY_FUNCTIONS    2
#define FUNC_GET_TEX_INFO       0
#define FUNC_UPDATE             1

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
        ctx.vbo_capacity += NEW_ENTS_PER_RESIZE * 20;
        ctx.ebo_capacity += NEW_ENTS_PER_RESIZE * 6;
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
    entity_get_tex_info(ent, &tex, &x1, &x2, &y1, &y2);
    for (i32 i = 0; i < 4; i++) {
        ctx.vbo_buffer[ctx.vbo_length++] = ent->position.x + dx[i] * ent->size.x;
        ctx.vbo_buffer[ctx.vbo_length++] = ent->position.y + dy[i] * ent->size.y;
        ctx.vbo_buffer[ctx.vbo_length++] = (dx[i] ^ ent->facing_left) ? x2 : x1;
        ctx.vbo_buffer[ctx.vbo_length++] = (dy[i]) ? y2 : y1;
        ctx.vbo_buffer[ctx.vbo_length++] = tex;
    }
    u32 idx = 4 * ctx.ebo_length / 6;
    for (i32 i = 0; i < 6; i++)
        ctx.ebo_buffer[ctx.ebo_length++] = winding[i] + idx;
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
}

void entity_context_clear_entities(void)
{
    while (!array_empty(ctx.entities)) {
        Entity* ent = array_pop(ctx.entities, 0);
        entity_destroy(ent);
    }
}

void entity_context_destroy(void)
{
    entity_context_clear_entities();
    array_destroy(ctx.entities);
    free(ctx.vbo_buffer);
    free(ctx.ebo_buffer);
    glDeleteVertexArrays(1, &ctx.entity_vao);
    glDeleteBuffers(1, &ctx.entity_vbo);
    glDeleteBuffers(1, &ctx.entity_ebo);
}

static bool collide(Entity* ent1, Entity* ent2)
{
    return !(
        ent1->position.x + ent1->size.x <= ent2->position.x
     || ent2->position.x + ent2->size.x <= ent1->position.x
     || ent1->position.y + ent1->size.y <= ent2->position.y
     || ent2->position.y + ent2->size.y <= ent1->position.y
    );
}

static void parse_hit(Entity* ent1, Entity* ent2)
{
    if (ent1->health <= 0 || ent2->health <= 0)
        return;

    if (!(ent1->friendly ^ ent2->friendly))
        return;

    if (ent1->id == ent2->id)
        return;

    if (ent1->id > ent2->id) {
        Entity* tmp = ent1;
        ent1 = ent2;
        ent2 = tmp;
    }

    if (ent2->id == ENT_PROJECTILE) {
        ent2->health = 0;
    }
} 

void entity_context_update(f32 dt)
{
    ctx.vbo_length = 0;
    ctx.ebo_length = 0;
    for (i32 i = 0; i < ctx.entities->length; i++) {
        Entity* ent1 = array_get(ctx.entities, i);
        for (i32 j = 0; j < i; j++) {
            Entity* ent2 = array_get(ctx.entities, j);
            if (collide(ent1, ent2))
                parse_hit(ent1, ent2);
        }
    }
    for (i32 i = ctx.entities->length - 1; i >= 0; i--) {
        Entity* ent = array_get(ctx.entities, i);
        if (ent->health <= 0) {
            array_pop(ctx.entities, i--);
            entity_destroy(ent);
            continue;
        }
        entity_update(ent, dt);
        push_entity_into_buffer(ent);
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
    glDrawElements(GL_TRIANGLES, ctx.ebo_length, GL_UNSIGNED_INT, 0);
}

const Array* entity_context_get_entities(void)
{
    return ctx.entities;
}

// ----------------------------------

#define CASE_CREATE(_ent, _func) \
    case _ent: _func(ent); break;

Entity* entity_create(EntityID id)
{
    Entity* ent = malloc(sizeof(Entity));
    array_push(ctx.entities, ent);
    ent->id = id;
    ent->frame = 0;
    ent->facing_left = FALSE;
    ent->grounded = FALSE;
    ent->position = ent->prev_position = vec2_create(0, 0);
    ent->velocity = vec2_create(0, 0);
    ent->size = vec2_create(1, 1);
    ent->friendly = FALSE;
    ent->health = 1;
    switch (id) {
        CASE_CREATE(ENT_PUBBLES, pubbles_create)
        CASE_CREATE(ENT_PROJECTILE, proj_create)
    }
    return ent;
}

#define CASE_UPDATE(_ent, _func) \
    case _ent: _func(ent, dt); break;

void entity_update(Entity* ent, f32 dt)
{
    ent->prev_position = ent->position;
    ent->position = vec2_add(ent->position, vec2_scale(ent->velocity, dt));
    if (ent->facing_left && ent->velocity.x > 0)
        ent->facing_left = FALSE;
    else if (!ent->facing_left && ent->velocity.x < 0)
        ent->facing_left = TRUE;
    switch (ent->id) {
        CASE_UPDATE(ENT_PUBBLES, pubbles_update)
        CASE_UPDATE(ENT_PROJECTILE, proj_update)
    }
}

#define CASE_GET_TEX_INFO(_ent, _func) \
    case _ent: _func(ent, tex, x1, x2, y1, y2); break;

void entity_get_tex_info(Entity* ent, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2)
{
    switch (ent->id) {
        CASE_GET_TEX_INFO(ENT_PUBBLES, pubbles_get_tex_info)
        CASE_GET_TEX_INFO(ENT_SHIRT, shirt_get_tex_info)
        CASE_GET_TEX_INFO(ENT_PROJECTILE, proj_get_tex_info)
        CASE_GET_TEX_INFO(ENT_SOCKS, socks_get_tex_info)
        CASE_GET_TEX_INFO(ENT_BOSS, boss_get_tex_info)
    }
}

#define CASE_DESTROY(_ent, _func) \
    case _ent: _func(ent); break;

void entity_destroy(Entity* ent)
{
    switch (ent->id) {
        CASE_DESTROY(ENT_PUBBLES, pubbles_destroy)
        CASE_DESTROY(ENT_PROJECTILE, proj_destroy)
    }
    free(ent);
}
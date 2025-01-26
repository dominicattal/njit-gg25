#include "game.h"
#include "../renderer/renderer.h"
#include "../window/window.h"
#include "entity.h"
#include "platform.h"
#include "background.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    Entity* player;
    u32 view_ubo;
    vec2 center;
    f32 zoom;
    f64 dt;
    bool started;
    pthread_t thread_id;
    bool kill_thread;
    sem_t mutex;
} Game;

static Game game;

static void collide_entity_platform(Entity* entity, Platform* platform)
{
    f32 ex1, ex2, ey1, ey2;
    f32 px1, px2, py1, py2;
    ex1 = entity->position.x; ex2 = entity->position.x + entity->size.x;
    ey1 = entity->position.y; ey2 = entity->position.y + entity->size.y;
    px1 = platform->position.x; px2 = platform->position.x + platform->size.x;
    py1 = platform->position.y; py2 = platform->position.y + platform->size.y;
    if (!(ex2 <= px1 || ex1 >= px2 || ey2 <= px1 || ey1 >= py2)) {
        if (entity->prev_position.x >= px2)
            entity->position.x = px2;
        else if (entity->prev_position.y >= py2) {
            entity->position.y = py2;
            entity->grounded = TRUE;
        }
        else if (entity->prev_position.x + entity->size.x <= px1)
            entity->position.x = px1 - entity->size.x;
        else {
            entity->position.y = py1 - entity->size.y;
            entity->grounded = TRUE;
        }
    }
}

static void collide_entities_platforms(f32 dt)
{
    Array* entities = entity_context_get_entities();
    Array* platforms = platform_context_get_platforms();
    for (i32 i = 0; i < entities->length; i++) {
        Entity* entity = array_get(entities, i);
        entity->grounded = FALSE;
        for (i32 j = 0; j < platforms->length; j++) {
            Platform* platform = array_get(platforms, j);
            collide_entity_platform(entity, platform);
        }
        if (!entity->grounded && entity->id != ENT_PROJECTILE) {
            entity->velocity.y -= 3 * dt;
        } else if (entity->grounded && entity->id == ENT_PROJECTILE) {
            entity->delete_flag = TRUE;
        }
    }
}

static void* game_update(void* vargp)
{
    f64 start;
    while (!game.kill_thread) {
        start = get_time();
        sem_wait(&game.mutex);
        entity_context_update((game.dt > 0.1) ? 0.1 : game.dt);
        platform_context_update((game.dt > 0.1) ? 0.1 : game.dt);
        collide_entities_platforms(game.dt);
        if (game.player != NULL) {
            game.center.x = game.player->position.x + game.player->size.x / 2;
            game.center.y = game.player->position.y;
        }
        background_context_update(game.center, game.zoom);
        sem_post(&game.mutex);
        game.dt = get_time() - start;
    }
}

void game_init(void)
{
    game.player = NULL;
    game.zoom = 0.2;
    game.dt = 0;
    glGenBuffers(1, &game.view_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, game.view_ubo);
    f32 ubo[] = {game.center.x, game.center.y, game.zoom, window_aspect_ratio()};
    glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(f32), ubo, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, UBO_VIEW, game.view_ubo);

    entity_context_init();
    platform_context_init();
    background_context_init();
    game.center = vec2_create(0, 0);
    game.kill_thread = FALSE;
    sem_init(&game.mutex, 0, 1);
    pthread_create(&game.thread_id, NULL, game_update, NULL);    
}

void game_destroy(void)
{
    game.kill_thread = TRUE;
    pthread_join(game.thread_id, NULL);
    sem_destroy(&game.mutex);
    glDeleteBuffers(1, &game.view_ubo);
    entity_context_destroy();
    background_context_destroy();
}

void game_move(vec2 dir, f32 dt)
{
    if (game.player != NULL) {
        game.player->velocity.x = 2 * dir.x;
        if (game.player->grounded && dir.y > 0)
            game.player->velocity.y = 2 * dir.y;
    }
    
}

void game_zoom(i32 mag, f32 dt)
{
    game.zoom += mag * dt;
    glBindBuffer(GL_UNIFORM_BUFFER, game.view_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(f32), sizeof(f32), &game.zoom);
}

void game_shoot(vec2 direction)
{
    if (game.player == NULL) 
        return;
    sem_wait(&game.mutex);
    Entity* proj = entity_create(ENT_PROJECTILE);
    proj->position = vec2_add(game.player->position, vec2_scale(game.player->size, 0.5));
    proj->velocity = vec2_scale(direction, 10);
    sem_post(&game.mutex);
}

void game_start(void)
{
    sem_wait(&game.mutex);
    game.player = entity_create(ENT_PUBBLES);
    game.player->position = vec2_create(game.center.x - game.player->size.x / 2, game.center.y);
    game.started = TRUE;
    Entity* ent = entity_create(ENT_SHIRT);
    ent->position = vec2_create(0.5, 1.0);
    Platform* platform = platform_create(PLATFORM_1);
    platform->position = vec2_create(-2, -2);
    platform->size = vec2_create(4, 0.5);
    sem_post(&game.mutex);
}

f64 game_dt(void)
{
    return game.dt;
}

void game_prepare_render(void)
{
    sem_wait(&game.mutex);
    entity_context_prepare_render();
    platform_context_prepare_render();
    background_context_prepare_render();
    sem_post(&game.mutex);
    glBindBuffer(GL_UNIFORM_BUFFER, game.view_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 2 * sizeof(f32), &game.center);
}

void game_render(void)
{
    if (!game.started)
        return;

    sem_wait(&game.mutex);
    background_context_render();
    platform_context_render();
    entity_context_render();
    sem_post(&game.mutex);
}
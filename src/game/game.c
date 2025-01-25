#include "game.h"
#include "../renderer/renderer.h"
#include "../window/window.h"
#include "entity.h"
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
    pthread_t thread_id;
    bool kill_thread;
    sem_t mutex;
} Game;

static Game game;

static void* game_update(void* vargp)
{
    f64 start;
    while (!game.kill_thread) {
        start = get_time();
        sem_wait(&game.mutex);
        entity_context_update(game.dt);
        sem_post(&game.mutex);
        game.dt = get_time() - start;
    }
}

void game_init(void)
{
    game.player = NULL;
    game.zoom = 0.3;
    game.dt = 0;
    glGenBuffers(1, &game.view_ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, game.view_ubo);
    f32 ubo[] = {game.center.x, game.center.y, game.zoom, window_aspect_ratio()};
    glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(f32), ubo, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, UBO_VIEW, game.view_ubo);

    entity_context_init();
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
}

void game_move(vec2 dir, f32 dt)
{
    game.center = vec2_add(game.center, vec2_scale(vec2_normalize(dir), dt));
    if (game.player != NULL)
        game.player->position = vec2_create(game.center.x - game.player->size.x / 2, game.center.y);
    glBindBuffer(GL_UNIFORM_BUFFER, game.view_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 2 * sizeof(f32), &game.center);
}

void game_zoom(i32 mag, f32 dt)
{
    game.zoom += mag * dt;
    glBindBuffer(GL_UNIFORM_BUFFER, game.view_ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(f32), sizeof(f32), &game.zoom);
}

void game_start(void)
{
    sem_wait(&game.mutex);
    game.player = entity_create(ENT_PUBBLES);
    Entity* ent1 = entity_create(ENT_PUBBLES);
    Entity* ent = entity_create(ENT_PUBBLES);
    ent->position = vec2_create(0.5, 0.5);
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
    sem_post(&game.mutex);
}

void game_render(void)
{    
    sem_wait(&game.mutex);
    entity_context_render();
    sem_post(&game.mutex);
}
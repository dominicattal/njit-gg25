#ifndef GAME_H
#define GAME_H

#include "../util/util.h"

void game_init(void);
void game_destroy(void);
f64  game_dt(void);
void game_start(void);

void game_move(vec2 dir, f32 dt);
void game_zoom(i32 mag, f32 dt);
void game_shoot(vec2 direction);

void game_prepare_render(void);
void game_render(void);

#endif
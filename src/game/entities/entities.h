#ifndef ENTITIES_H
#define ENTITIES_H

#include "../entity.h"

void pubbles_create(Entity* ent);
void pubbles_get_tex_info(Entity* ent, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2);
void pubbles_update(Entity* ent, f32 dt);
void pubbles_destroy(Entity* ent);

void shirt_create(Entity* ent);
void shirt_get_tex_info(Entity* ent, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2);
void shirt_update(Entity* ent, f32 dt);
void shirt_destroy(Entity* ent);

void pants_create(Entity* ent);
void pants_get_tex_info(Entity* ent, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2);
void pants_update(Entity* ent, f32 dt);
void pants_destroy(Entity* ent);

void boss_create(Entity* ent);
void boss_get_tex_info(Entity* ent, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2);
void boss_update(Entity* ent, f32 dt);
void boss_destroy(Entity* ent);

void boss_create(Entity* ent);
void boss_get_tex_info(Entity* ent, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2);
void boss_update(Entity* ent, f32 dt);
void boss_destroy(Entity* ent);

#endif
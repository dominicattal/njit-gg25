#ifndef ENTITIES_H
#define ENTITIES_H

#include "../entity.h"

void pubbles_create(Entity* ent);
void pubbles_get_tex_info(Entity* ent, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2);
void pubbles_update(Entity* ent, f32 dt);
void pubbles_destroy(Entity* ent);

#endif
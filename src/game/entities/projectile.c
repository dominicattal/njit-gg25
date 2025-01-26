#include "entities.h"
#include "../../renderer/texture.h"

void proj_create(Entity* ent)
{
    ent->size = vec2_create(0.1, 0.1);
    ent->health = 0.2;
}

void proj_get_tex_info(Entity* ent, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2)
{
    *x1 = 0;
    *x2 = 1;
    *y1 = 1;
    *y2 = 0;
    *tex = TEX_COLOR;
}

void proj_update(Entity* ent, f32 dt)
{
    ent->health -= dt;
    if (ent->health < 0)
        ent->health = 0;
}

void proj_destroy(Entity* ent)
{

}
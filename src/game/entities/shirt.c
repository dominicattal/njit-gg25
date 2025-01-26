#include "entities.h"
#include "../../renderer/texture.h"

void shirt_create(Entity* ent)
{

}

void shirt_get_tex_info(Entity* ent, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2)
{
    *x1 = 0;
    *x2 = 1;
    *y1 = 1;
    *y2 = 0;
    *tex = TEX_COLOR;
}

void shirt_update(Entity* ent, f32 dt)
{

}

void shirt_destroy(Entity* ent)
{

}
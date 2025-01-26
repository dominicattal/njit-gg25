#include "entities.h"
#include "../../renderer/texture.h"

typedef struct {
    f32 timer;
} Data;

static Data* data_create(void)
{
    Data* data = malloc(sizeof(Data));
    data->timer = 0.5;
}

void proj_create(Entity* ent)
{
    Data* data = data_create();
    ent->size = vec2_create(0.1, 0.1);
    ent->data = data;
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
    Data* data = ent->data;
    data->timer -= dt;
    if (data->timer < 0)
        ent->delete_flag = TRUE;
}

void proj_destroy(Entity* ent)
{
    free(ent->data);
}
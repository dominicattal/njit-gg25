#include "../../util/util.h"
#include "../../renderer/texture.h"
#include "../entity.h"
#include <stdlib.h>

#define WIDTH  96.0f
#define HEIGHT 128.0f
#define create_rect(x, y, w, h) \
    {(f32)x/WIDTH, (f32)(x+w)/WIDTH, (f32)(y+h)/HEIGHT, (f32)y/HEIGHT}

typedef struct {
    f32 timer;
} Data;

void pubbles_get_tex_info(Entity* ent, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2)
{
    static f32 frames[10][4] = {
        create_rect(0, 0, 32, 32),
        create_rect(32, 0, 32, 32),
        create_rect(64, 0, 32, 32),
        create_rect(0, 32, 32, 32),
        create_rect(32, 32, 32, 32),
        create_rect(64, 32, 32, 32),
        create_rect(0, 64, 32, 32),
        create_rect(32, 64, 32, 32),
        create_rect(64, 64, 32, 32),
        create_rect(0, 96, 32, 32),
    };

    *x1 = frames[ent->frame][0];
    *x2 = frames[ent->frame][1];
    *y1 = frames[ent->frame][2];
    *y2 = frames[ent->frame][3];
    *tex = TEX_PUBBLES;
}

void pubbles_create(Entity* entity)
{
    Data* data = malloc(sizeof(Data));
    entity->data = data;
    data->timer = 0;
    entity->position = vec2_create(0, 0);
    entity->direction = vec2_create(0, 0);
    entity->speed = 10;
    entity->size = vec2_create(1, 1);
}

void pubbles_update(Entity* entity, f32 dt)
{
    Data* data = entity->data;
    data->timer -= dt;
    if (data->timer < 0) {
        data->timer += 0.04;
        entity->frame = (entity->frame + 1) % 10;
    }
}

void pubbles_destroy(Entity* entity)
{
    free(entity->data);
}
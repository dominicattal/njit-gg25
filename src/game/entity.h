#ifndef ENTITY_H
#define ENTITY_H

#include "../util/util.h"

#define NUM_ENTITIES 5
typedef enum {
    ENT_PUBBLES = 0,
    ENT_SHIRT,
    ENT_PANT,
    ENT_SOCKS,
    ENT_BOSS
} EntityID;

typedef struct {
    u32 id;
    u32 frame;
    vec2 position;
    vec2 prev_position;
    vec2 direction;
    f32 speed;
    vec2 size;
    bool facing_left;
    void* data;
} Entity;

void entity_context_init(void);
void entity_context_update(f32 dt);
void entity_context_prepare_render(void);
void entity_context_render(void);
void entity_context_destroy(void);

const Array* entity_context_get_entities(void);

Entity* entity_create(EntityID id);
void entity_update(Entity* entity, f32 dt);
void entity_destroy(Entity* entity);
void entity_get_tex_info(Entity* ent, u32* tex, f32* x1, f32* x2, f32* y1, f32* y2);

#endif
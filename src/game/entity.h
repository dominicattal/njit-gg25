#ifndef ENTITY_H
#define ENTITY_H

#include "../util/util.h"

typedef enum {
    ENT_PUBBLES = 0
} EntityID;

typedef struct {
    u32 id;
    u32 frame;
    f32 timer;
} Entity;

void entity_context_init(void);
void entity_context_update(f32 dt);
void entity_context_prepare_render(void);
void entity_context_render(void);
void entity_context_destroy(void);

Entity* entity_create(EntityID id);
void entity_update(Entity* entity, f32 dt);
void entity_destroy(Entity* entity);

#endif
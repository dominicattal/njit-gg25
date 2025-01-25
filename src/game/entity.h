#ifndef ENTITY_H
#define ENTITY_H

#include "../util/util.h"

typedef enum {
    ENT_PUBBLES = 0
} EntityID;

typedef struct {
    u32 id;
    u32 frame;
    u32 buffer_idx;
} Entity;

void entity_context_init(void);
void entity_context_prepare_render(void);
void entity_context_render(void);
void entity_context_destroy(void);

Entity* entity_create(EntityID id);
void entity_destroy(Entity* entity);

#endif
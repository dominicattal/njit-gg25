#ifndef ENTITY_H
#define ENTITY_H

#include "../util/util.h"

typedef enum {
    ENT_PUBBLES = 0
} EntityID;

void entity_init(void);
void entity_prepare_render(void);
void entity_render(void);
void entity_destroy(void);

#endif
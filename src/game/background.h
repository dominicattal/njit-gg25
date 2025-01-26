#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../util/util.h"

#define NUM_BACKGROUNDS 2

typedef enum {
    BACKGROUND_1 = 0,
    BACKGROUND_2 
} Background;

void background_context_init(void);
void background_context_update(vec2 center, f32 zoom);
void background_context_destroy(void);

void background_context_prepare_render(void);
void background_context_render(void);

#endif
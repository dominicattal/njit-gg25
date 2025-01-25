#include "components.h"
#include <stdlib.h>

#define NUM_FRAMES 5
static u32 tex_frames[NUM_FRAMES] = {
    TEX_CUTSCENE_1,
    TEX_CUTSCENE_2,
    TEX_CUTSCENE_3,
    TEX_CUTSCENE_4,
    TEX_CUTSCENE_5
};

typedef struct {
    u32 frame;
    Component* left_button;
    Component* right_button;
} Data;

static Data* data_create(void)
{
    Data* data = malloc(sizeof(Data));
    data->frame = 0;
    data->left_button = NULL;
    data->right_button = NULL;
    return data;
}

void comp_slideshow_init(Component* comp)
{
    comp->data = data_create();
}

void comp_slideshow_destroy(Component* comp)
{
    free(comp->data);
}


#include "components.h"
#include "../gui.h"
#include "../../game/game.h"
#include <glfw.h>
#include <stdlib.h>

#define NUM_FRAMES 5
static u32 tex_frames[NUM_FRAMES+1] = {
    TEX_CUTSCENE_1,
    TEX_CUTSCENE_2,
    TEX_CUTSCENE_3,
    TEX_CUTSCENE_4,
    TEX_CUTSCENE_5,
    TEX_NONE
};

typedef struct {
    u32 frame;
} Data;

static Data* data_create(void)
{
    Data* data = malloc(sizeof(Data));
    data->frame = 0;
    return data;
}

void comp_slideshow_init(Component* comp)
{
    comp->data = data_create();
}

void comp_slideshow_update(Component* comp, f64 dt)
{
    Data* data = comp->data;
    if (data->frame >= NUM_FRAMES) {
        gui_load(GUI_GAME);
        game_start();
    }
    else
        comp_set_tex(comp, tex_frames[data->frame]);
}

void comp_slideshow_key(Component* comp, i32 key, i32 scancode, i32 action, i32 mods)
{
    Data* data = comp->data;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        data->frame++;
}

void comp_slideshow_destroy(Component* comp)
{
    free(comp->data);
}


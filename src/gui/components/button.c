#include "components.h"
#include "../../renderer/renderer.h"
#include "../../window/window.h"
#include "../../game/game.h"
#include <glfw.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void (*func_ptr)(void);
} Data;

static Data* data_create(void)
{
    Data* data = malloc(sizeof(Data));
    data->func_ptr = NULL;
    return data;
}

void comp_button_set_function(Component* comp, void (*func_ptr)(void))
{
    Data* data = comp->data;
    data->func_ptr = func_ptr;
}

void comp_button_init(Component* comp)
{
    comp_set_is_text(comp, TRUE);
    comp_set_font(comp, FONT_MONOSPACE);
    comp_set_halign(comp, ALIGN_LEFT);
    comp_set_valign(comp, ALIGN_TOP);
    comp_set_font_size(comp, 16);
    comp_set_color(comp, 255, 255, 255, 0);
    comp->data = data_create();
}

void comp_button_click(Component* comp, i32 button, i32 action)
{
    Data* data = comp->data;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        data->func_ptr();
}

void comp_button_key(Component* comp, i32 key, i32 scancode, i32 action, i32 mods)
{

}

void comp_button_hover(Component* comp, bool status)
{

}

void comp_button_destroy(Component* comp)
{
    free(comp->data);
}
#include "components.h"
#include "../../window/window.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <glfw.h>

void comp_textbox_init(Component* comp)
{
    comp_set_is_text(comp, TRUE);
}

void comp_textbox_hover(Component* comp, bool status)
{
    if (comp_is_hovered(comp) && !status) {
        comp_set_hovered(comp, FALSE);
        comp_set_color(comp, 0, 255, 0, 255);
    } else if (!comp_is_hovered(comp) && status) {
        comp_set_hovered(comp, TRUE);
        comp_set_color(comp, 0, 255, 255, 255);
    }
}

void comp_textbox_click(Component* comp, i32 button, i32 action)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        comp_set_color(comp, rand() % 256, rand() % 256, rand() % 256, 255);
}

void comp_textbox_key(Component* comp, i32 key, i32 scancode, i32 action, i32 mods)
{
    
}

void comp_textbox_update(Component* comp, f64 dt)
{

}

void comp_textbox_destroy(Component* comp)
{

}
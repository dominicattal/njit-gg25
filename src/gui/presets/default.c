#include "presets.h"

void gui_preset_default(Component* root)
{
    i32 width, height;
    comp_get_size(root, &width, &height);
    Component* textbox = comp_create(50, 50, 150, 150, COMP_TEXTBOX);
    comp_set_color(textbox, 255, 0, 255, 120);
    comp_set_text(textbox, "Hello World");
    comp_attach(root, textbox);
}
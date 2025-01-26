#include "presets.h"

void gui_preset_slideshow(Component* root)
{
    i32 width, height;
    comp_get_size(root, &width, &height);
    Component* slideshow = comp_create(460, 40, 1000, 1000, COMP_SLIDESHOW);
    comp_attach(root, slideshow);
}

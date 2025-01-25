#include "presets.h"
#include "../../game/game.h"

void gui_preset_game(Component* root)
{
    i32 width, height;
    comp_get_size(root, &width, &height);
    Component* textbox = comp_create(20, height-220, 500, 200, COMP_TEXTBOX);
    comp_set_font(textbox, FONT_BUBBLE);
    comp_set_halign(textbox, ALIGN_CENTER);
    comp_set_valign(textbox, ALIGN_CENTER);
    comp_set_font_size(textbox, 128);
    comp_set_color(textbox, 255, 0, 255, 75);
    comp_set_clickable(textbox, TRUE);
    comp_set_text(textbox, "Score");
    comp_attach(root, textbox);
}

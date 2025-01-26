#include "presets.h"
#include "../../game/game.h"
#include "../gui.h"

void start_slideshow(void)
{  
    gui_load(GUI_SLIDESHOW);
}

void gui_preset_default(Component* root)
{
    i32 width, height;
    comp_get_size(root, &width, &height);
    Component* textbox = comp_create(width/2-400, height-150-80, 800, 150, COMP_TEXTBOX);
    comp_set_font(textbox, FONT_BUBBLE);
    comp_set_halign(textbox, ALIGN_CENTER);
    comp_set_valign(textbox, ALIGN_CENTER);
    comp_set_font_size(textbox, 128);
    comp_set_color(textbox, 255, 0, 255, 75);
    comp_set_clickable(textbox, TRUE);
    comp_set_text(textbox, "PUBBLES");
    comp_attach(root, textbox);

    Component* play_button = comp_create(200, 200, 300, 150, COMP_BUTTON);
    comp_set_font(play_button, FONT_BUBBLE);
    comp_set_halign(play_button, ALIGN_CENTER);
    comp_set_valign(play_button, ALIGN_CENTER);
    comp_set_font_size(play_button, 96);
    comp_set_color(play_button, 255, 0, 255, 75);
    comp_set_clickable(play_button, TRUE);
    comp_set_text(play_button, "PLAY");
    comp_button_set_function(play_button, start_slideshow);
    comp_attach(root, play_button);
}

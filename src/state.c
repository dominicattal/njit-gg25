#include "state.h"
#include "util/util.h"
#include <stdio.h>
#include "game/game.h"
#include "renderer/renderer.h"
#include "window/window.h"
#include "gui/gui.h"
#include "audio/audio.h"
#include <glfw.h>

void state_init(void)
{
    window_init();
    renderer_init();
    audio_init();
    gui_init();
    game_init();

    gui_load(GUI_GAME);
    game_start();
}

static void process_input(f32 dt)
{
    i32 zoom_magnitude = 0;
    vec2 direction = vec2_create(0, 0);

    if (window_key_pressed(GLFW_KEY_W))
        direction.y++;
    if (window_key_pressed(GLFW_KEY_S))
        direction.y--;
    if (window_key_pressed(GLFW_KEY_A))
        direction.x--;
    if (window_key_pressed(GLFW_KEY_D))
        direction.x++;
    if (window_key_pressed(GLFW_KEY_O))
        zoom_magnitude++;
    if (window_key_pressed(GLFW_KEY_P))
        zoom_magnitude--;

    game_move(direction, dt);
    if (zoom_magnitude != 0)
        game_zoom(zoom_magnitude, dt);
}

void state_loop(void)
{
    f64 dt = 0, time;
    while (!window_closed()) 
    {
        time = get_time();
        process_input(dt);
        gui_update(dt);
        window_update();
        renderer_render();
        dt = get_time() - time;
    }
}

void state_destroy(void)
{
    window_destroy();
    audio_destroy();
    gui_destroy();
    game_destroy();
    renderer_destroy();
}
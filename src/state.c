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
}

static void process_input(f32 dt)
{
    
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
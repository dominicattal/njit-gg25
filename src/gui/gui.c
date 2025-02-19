#include "gui.h"
#include "component.h"
#include "presets/presets.h"
#include "../renderer/renderer.h"
#include "../window/window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>

typedef struct {
    u32 comp_vbo_length, comp_vbo_max_length;
    f32* comp_vbo_buffer;
    u32 comp_ebo_length, comp_ebo_max_length;
    u32* comp_ebo_buffer;
    u32 font_vbo_length, font_vbo_max_length;
    f32* font_vbo_buffer;
    u32 font_ebo_length, font_ebo_max_length;
    u32* font_ebo_buffer;
    u32 comp_ebo, font_ebo;
    u32 comp_vbo, font_vbo;
    u32 comp_vao, font_vao;
} GUIData;

typedef struct {
    Component* root;
    GUIData data;
    bool initialized;
    bool load_flag;
    GUIPreset current_preset;
} GUI;

static GUI gui;

static void (*preset_functions[NUM_PRESETS])();

static void gui_loader_init(void);

void gui_init(void)
{
    comp_init();
    gui_loader_init();

    i32 xres, yres;
    window_get_resolution(&xres, &yres);
    gui.root = comp_create(0, 0, xres, yres, COMP_DEFAULT);
    comp_set_color(gui.root, 0, 0, 0, 0);
    comp_set_hoverable(gui.root, FALSE);
    gui.initialized = TRUE;

    glGenVertexArrays(1, &gui.data.comp_vao);
    glGenVertexArrays(1, &gui.data.font_vao);
    glGenBuffers(1, &gui.data.comp_ebo);
    glGenBuffers(1, &gui.data.font_ebo);
    glGenBuffers(1, &gui.data.comp_vbo);
    glGenBuffers(1, &gui.data.font_vbo);

    glBindVertexArray(gui.data.comp_vao);
    glBindBuffer(GL_ARRAY_BUFFER, gui.data.comp_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(0 * sizeof(f32)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(2 * sizeof(f32)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(4 * sizeof(f32)));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(8 * sizeof(f32)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindVertexArray(gui.data.font_vao);
    glBindBuffer(GL_ARRAY_BUFFER, gui.data.font_vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(0 * sizeof(f32)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(2 * sizeof(f32)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(4 * sizeof(f32)));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(f32), (void*)(8 * sizeof(f32)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
}

static void update_components_helper(Component* comp, f64 dt)
{
    comp_update(comp, dt);
    for (i32 i = 0; i < comp_num_children(comp); i++)
        update_components_helper(comp->children[i], dt);
}

void gui_update(f64 dt)
{
    if (gui.load_flag) {
        comp_destroy_children(gui.root);
        preset_functions[gui.current_preset](gui.root);
        gui.load_flag = FALSE;
    }
    update_components_helper(gui.root, dt);
}

void gui_destroy(void)
{
    if (!gui.initialized)
        return;
    
    comp_destroy(gui.root);
    free(gui.data.comp_vbo_buffer);
    free(gui.data.comp_ebo_buffer);
    free(gui.data.font_vbo_buffer);
    free(gui.data.font_ebo_buffer);
    glDeleteBuffers(1, &gui.data.comp_ebo);
    glDeleteBuffers(1, &gui.data.font_ebo);
    glDeleteBuffers(1, &gui.data.comp_vbo);
    glDeleteBuffers(1, &gui.data.font_vbo);
    glDeleteVertexArrays(1, &gui.data.comp_vao);
    glDeleteVertexArrays(1, &gui.data.font_vao);
}

static void gui_mouse_button_callback_helper(Component* comp, i32 button, i32 action)
{
    i32 x, y, w, h;
    comp_get_bbox(comp, &x, &y, &w, &h);

    if (comp_is_clickable(comp) && window_cursor_in_bbox(x, y, w, h))
        comp_click(comp, button, action);

    for (i32 i = 0; i < comp_num_children(comp); i++)
        gui_mouse_button_callback_helper(comp->children[i], button, action);
}

void gui_mouse_button_callback(i32 button, i32 action)
{
    gui_mouse_button_callback_helper(gui.root, button, action);
}

static void gui_key_callback_helper(Component* comp, i32 key, i32 scancode, i32 action, i32 mods)
{
    comp_key(comp, key, scancode, action, mods);
    for (i32 i = 0; i < comp_num_children(comp); i++)
        gui_key_callback_helper(comp->children[i], key, scancode, action, mods);
}

void gui_key_callback(i32 key, i32 scancode, i32 action, i32 mods) 
{
    gui_key_callback_helper(gui.root, key, scancode, action, mods);
}

static void gui_cursor_callback_helper(Component* comp)
{
    i32 x, y, w, h;
    comp_get_bbox(comp, &x, &y, &w, &h);

    if (comp_is_hoverable(comp))
        comp_hover(comp, window_cursor_in_bbox(x, y, w, h));

    for (i32 i = 0; i < comp_num_children(comp); i++)
        gui_cursor_callback_helper(comp->children[i]);
}

void gui_cursor_callback(void) 
{
    gui_cursor_callback_helper(gui.root);
}

/* ------------------------------ */

static void gui_loader_init(void)
{
    preset_functions[GUI_MAIN_MENU] = gui_preset_default;
    preset_functions[GUI_SLIDESHOW] = gui_preset_slideshow;
    preset_functions[GUI_GAME] = gui_preset_game;
}

void gui_load(GUIPreset preset)
{
    gui.current_preset = preset;
    gui.load_flag = TRUE;
}

/* ------------------------------ */

#define FLOAT_PER_VERTEX 9
#define NUM_VERTICES     4
#define NUM_FLOATS       NUM_VERTICES * FLOAT_PER_VERTEX
#define NUM_INDEXES      6

static void resize_comp_buffers(u32 num_components)
{
    if (gui.data.comp_vbo_buffer == NULL) {
        gui.data.comp_vbo_max_length += NUM_FLOATS * num_components;
        gui.data.comp_ebo_max_length += NUM_INDEXES * num_components;
        gui.data.comp_vbo_buffer = malloc(gui.data.comp_vbo_max_length * sizeof(f32));
        gui.data.comp_ebo_buffer = malloc(gui.data.comp_ebo_max_length * sizeof(u32));
    }
    else if (gui.data.comp_vbo_length + NUM_FLOATS * num_components >= gui.data.comp_vbo_max_length) {
        gui.data.comp_vbo_max_length += NUM_FLOATS * num_components;
        gui.data.comp_ebo_max_length += NUM_INDEXES * num_components;
        gui.data.comp_vbo_buffer = realloc(gui.data.comp_vbo_buffer, gui.data.comp_vbo_max_length * sizeof(f32));
        gui.data.comp_ebo_buffer = realloc(gui.data.comp_ebo_buffer, gui.data.comp_ebo_max_length * sizeof(u32));
    }
}

static void resize_font_buffers(u32 num_glyphs)
{
    if (gui.data.font_vbo_buffer == NULL) {
        gui.data.font_vbo_max_length += NUM_FLOATS * num_glyphs;
        gui.data.font_ebo_max_length += NUM_INDEXES * num_glyphs;
        gui.data.font_vbo_buffer = malloc(gui.data.font_vbo_max_length * sizeof(f32));
        gui.data.font_ebo_buffer = malloc(gui.data.font_ebo_max_length * sizeof(u32));
    }
    if (gui.data.font_vbo_length + NUM_FLOATS * num_glyphs >= gui.data.font_vbo_max_length) {
        gui.data.font_vbo_max_length += NUM_FLOATS * num_glyphs;
        gui.data.font_ebo_max_length += NUM_INDEXES * num_glyphs;
        gui.data.font_vbo_buffer = realloc(gui.data.font_vbo_buffer, gui.data.font_vbo_max_length * sizeof(f32));
        gui.data.font_ebo_buffer = realloc(gui.data.font_ebo_buffer, gui.data.font_ebo_max_length * sizeof(u32));
    }
}

#define A gui.data.font_vbo_buffer[gui.data.font_vbo_length++]
#define B gui.data.font_ebo_buffer[gui.data.font_ebo_length++]

static void update_data_text(Component* comp)
{
    if (!comp_is_text(comp))
        return;

    if (comp->text == NULL)
        return;

    i32 cx, cy, cw, ch;     // comp position and size
    f32 x1, y1, x2, y2;     // screen coordinates
    f32 u1, v1, u2, v2;     // bitmap coordinates
    i32 a1, b1, a2, b2;     // glyph bounding box
    i32 x, y, w, h;         // pixel coordinates
    i32 ascent, descent;    // highest and lowest glyph offsets
    i32 line_gap;           // gap between lines
    i32 adv, lsb, kern;     // advance, left side bearing, kerning
    u8  ha, va;             // horizontal and vertical alignment
    u8  justify;            // branchless justify
    i32 font_size;          // font_size = ascent - descent
    Font font;              // font
    i32 num_spaces;         // count whitespace for horizontal alignment
    f32 dy;                 // change in y for vertical alignment
    u32 ebo_idx, vbo_idx;   // ebo index of current glyph, vbo index of first glyph
    i32 length;             // index in text, length of text
    char* text;             // text, equal to comp->text
    u32 tex_id;             // texture id for use in shader

    register i32 ox, oy, test_ox;    // glyph origin
    register i32 prev_test_ox;       // edge case
    register i32 left, right, mid;   // pointers for word
    
    comp_get_position(comp, &cx, &cy);
    comp_get_size(comp, &cw, &ch);
    comp_get_align(comp, &ha, &va);
    comp_get_font(comp, &font);
    comp_get_font_size(comp, &font_size);
    text = comp->text;
    length = strlen(text);
    
    justify = 0;
    if (ha == ALIGN_JUSTIFY) {
        ha = ALIGN_LEFT;
        justify = 1;
    }
    
    font_info(font, font_size, &ascent, &descent, &line_gap);
    
    left = right = 0;
    ox = 0;
    oy = ascent;
    resize_font_buffers(length);
    vbo_idx = gui.data.font_vbo_length;
    while (right < length) {
        
        while (right < length && (text[right] == ' ' || text[right] == '\t' || text[right] == '\n'))
            right++;

        left = right;
        prev_test_ox = test_ox = 0;
        num_spaces = 0;
        while (right < length && text[right] != '\n' && test_ox <= cw) {
            font_char_hmetrics(font, font_size, text[right], &adv, &lsb);
            font_char_kern(font, font_size, text[right], text[right+1], &kern);
            prev_test_ox = test_ox;
            test_ox += adv + kern;
            num_spaces += text[right] == ' ';
            right++;
        }

        mid = right;
        if (test_ox > cw) {
            while (mid > left && text[mid-1] != ' ') {
                font_char_hmetrics(font, font_size, text[mid-1], &adv, &lsb);
                font_char_kern(font, font_size, text[mid-1], text[mid], &kern);
                test_ox -= adv + kern;
                mid--;
            }
            while (mid > left && text[mid-1] == ' ') {
                font_char_hmetrics(font, font_size, text[mid-1], &adv, &lsb);
                font_char_kern(font, font_size, text[mid-1], text[mid], &kern);
                test_ox -= adv + kern;
                num_spaces -= text[mid-1] == ' ';
                mid--;
            }
        }

        if (mid == left) {
            if (left == right) {
                test_ox = cw;
                right = left + 1;
            } else {
                test_ox = prev_test_ox;
                right = right - 1;
            }
        }
        else {
            right = mid;
        }

        if (left == right)
            right++;

        if (text[right-1] != ' ') {
            font_char_hmetrics(font, font_size, text[right-1], &adv, &lsb);
            font_char_bbox(font, font_size, text[right-1], &a1, &b1, &a2, &b2);
            test_ox -= adv - (a2 + a1);
        }
        
        ox = ha * (cw - test_ox) / 2.0f;

        while (left < right) {
            font_char_hmetrics(font, font_size, text[left], &adv, &lsb);
            font_char_bbox(font, font_size, text[left], &a1, &b1, &a2, &b2);
            font_char_bmap(font, font_size, text[left], &u1, &v1, &u2, &v2);
            font_char_kern(font, font_size, text[left], text[left+1], &kern);

            x = cx + ox + lsb;
            y = cy + ch - oy - b2;
            w = a2 - a1;
            h = b2 - b1;

            window_pixel_to_screen_bbox(x, y, w, h, &x1, &y1, &x2, &y2);

            ebo_idx = gui.data.font_vbo_length / FLOAT_PER_VERTEX;
            tex_id = texture_font_id(font, text[left]);

            if (text[left] != '\0' && text[left] != ' ') {
                A = x1, A = y1, A = 0, A = 1, A = 0, A = 0, A = 0, A = 1, A = tex_id;
                A = x1, A = y2, A = 0, A = 0, A = 0, A = 0, A = 0, A = 1, A = tex_id;
                A = x2, A = y2, A = 1, A = 0, A = 0, A = 0, A = 0, A = 1, A = tex_id;
                A = x2, A = y1, A = 1, A = 1, A = 0, A = 0, A = 0, A = 1, A = tex_id;
                B = ebo_idx, B = ebo_idx + 2, B = ebo_idx + 1, 
                B = ebo_idx, B = ebo_idx + 3, B = ebo_idx + 2;
            }   

            ox += adv + kern;
            if (justify && text[left] == ' ')
                ox += (cw - test_ox) / num_spaces;

            left++;
        }

        oy += ascent - descent + line_gap;
    }

    if (va == ALIGN_TOP)
        return;

    oy -= ascent - descent + line_gap;
    window_pixel_to_screen_y(va * (ch - oy) / 2, &dy);

    while (vbo_idx < gui.data.font_vbo_length) {
        gui.data.font_vbo_buffer[vbo_idx + 1] -= dy;
        vbo_idx += FLOAT_PER_VERTEX;
    }
}

#undef A
#undef B

#define A gui.data.comp_vbo_buffer[gui.data.comp_vbo_length++]
#define B gui.data.comp_ebo_buffer[gui.data.comp_ebo_length++]

static void update_data_helper(Component* comp)
{
    if (!comp_is_visible(comp)) 
        return;
    
    i32 cx, cy, cw, ch;
    u8  cr, cg, cb, ca;
    f32 x1, y1, x2, y2, r, g, b, a;
    u32 idx = gui.data.comp_vbo_length / FLOAT_PER_VERTEX;
    comp_get_position(comp, &cx, &cy);
    comp_get_size(comp, &cw, &ch);
    comp_get_color(comp, &cr, &cg, &cb, &ca);
    resize_comp_buffers(1);
    window_pixel_to_screen_bbox(cx, cy, cw, ch, &x1, &y1, &x2, &y2);
    r = cr / 255.0f, g = cg / 255.0f, b = cb / 255.0f, a = ca / 255.0f;

    A = x1, A = y1, A = 0.0, A = 1.0, A = r, A = g, A = b, A = a, A = comp_tex(comp);
    A = x1, A = y2, A = 0.0, A = 0.0, A = r, A = g, A = b, A = a, A = comp_tex(comp);
    A = x2, A = y2, A = 1.0, A = 0.0, A = r, A = g, A = b, A = a, A = comp_tex(comp);
    A = x2, A = y1, A = 1.0, A = 1.0, A = r, A = g, A = b, A = a, A = comp_tex(comp);
    B = idx, B = idx + 2, B = idx + 1, B = idx, B = idx + 3, B = idx + 2;

    update_data_text(comp);
    for (i32 i = 0; i < comp_num_children(comp); i++)
        update_data_helper(comp->children[i]);
}

#undef A
#undef B

static void update_data(void)
{
    gui.data.comp_vbo_length = gui.data.comp_ebo_length = 0;
    gui.data.font_vbo_length = gui.data.font_ebo_length = 0;
    update_data_helper(gui.root);
}

void gui_prepare_render(void)
{
    update_data();
    glBindBuffer(GL_ARRAY_BUFFER, gui.data.comp_vbo);
    glBufferData(GL_ARRAY_BUFFER, gui.data.comp_vbo_length * sizeof(f32), gui.data.comp_vbo_buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui.data.comp_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gui.data.comp_ebo_length * sizeof(u32), gui.data.comp_ebo_buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, gui.data.font_vbo);
    glBufferData(GL_ARRAY_BUFFER, gui.data.font_vbo_length * sizeof(f32), gui.data.font_vbo_buffer, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui.data.font_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gui.data.font_ebo_length * sizeof(u32), gui.data.font_ebo_buffer, GL_STATIC_DRAW);
}

void gui_render(void)
{
    shader_use(SHADER_GUI);

    glBindVertexArray(gui.data.comp_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui.data.comp_ebo);
    glDrawElements(GL_TRIANGLES, gui.data.comp_ebo_length, GL_UNSIGNED_INT, 0);

    glBindVertexArray(gui.data.font_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gui.data.font_ebo);
    glDrawElements(GL_TRIANGLES, gui.data.font_ebo_length, GL_UNSIGNED_INT, 0);
}
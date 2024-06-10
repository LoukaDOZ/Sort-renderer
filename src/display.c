#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "sort.h"
#include "render.h"
#include "functions.h"
#include "utils.h"
#include "display.h"

#define DELAY_STEP 10000

#define CIRCLE_MIN_TRIANGLES 36
#define SNAIL_DRAWING_CORRECTION_ANGLE 0.01f

#define NAME_TEXT "Sort"
#define COMPLEXITY_TEXT "Average complexity"
#define TIME_TEXT "Time"
#define CORRECTED_TIME_TEXT "Corrected time"

#define SHUFFLE_TEXT "Shuffling"
#define SHUFFLE_TEXT_LEN 9

#define VALIDATE_TEXT "Validating"
#define VALIDATE_TEXT_LEN 10

#define PAUSE_TEXT "Paused"
#define PAUSE_TEXT_LEN 6

#define TEXT_LETTER_W 8
#define TEXT_H 15

#define TEXT_BUFFER_SIZE 100
char TEXT_BUFFER[TEXT_BUFFER_SIZE];

#define TIME_BUFFER_SIZE 50
char TIME_BUFFER[TIME_BUFFER_SIZE];

typedef bool (*Draw_array_function)(Render* render, Shared_data shared_data, int window_w, int window_h, bool colorized);

const short BAR_DRAWING = 0;
const short DOT_DRAWING = 1;
const short CIRCLE_DRAWING = 2;
const short SNAIL_DRAWING = 3;

int add_by_until(long* delay, int nb_add, long min, long max, long step) {
    if(*delay != min && *delay != max && *delay % max == 0)
        return nb_add;

    long limit = nb_add < 0 ? min : max;
    int until_limit = (limit - *delay) / step;
    int count = abs(until_limit) < abs(nb_add) ? until_limit : nb_add;

    *delay += step * count;
    return nb_add - count;
}

void handle_render_events(Render* render, Shared_data shared_data, bool* paused, bool* show_info) {
    handle_events(render);

    int nb_up = was_pressed(render, MOUSE_WHEEL_UP) + was_pressed(render, KEY_ARROW_UP);
    int nb_down = was_pressed(render, MOUSE_WHEEL_DOWN) + was_pressed(render, KEY_ARROW_DOWN);
    int nb_next = was_pressed(render, KEY_ARROW_RIGHT) - was_pressed(render, KEY_ARROW_LEFT);

    if(do_quit(render) || was_pressed(render, KEY_Q) > 0) {
        set_has_quitted(shared_data, true);
        return;
    }

    if(was_pressed(render, KEY_R) > 0)
        set_has_restarted(shared_data, true);

    if(was_pressed(render, KEY_I) % 2 > 0)
        *show_info = inverse_bool(*show_info);

    if(nb_next != 0)
        set_sort_algo_index(shared_data, nb_next);

    if(was_pressed(render, KEY_P) % 2 > 0) {
        *paused = inverse_bool(is_paused(shared_data));
        set_is_paused(shared_data, *paused);
    }

    if(nb_up != nb_down) {
        long delay = get_simulation_delay(shared_data);
        int nb_add = nb_down - nb_up;

        if(nb_add > 0) {
            if(delay <= 0) {
                delay = 1;
                nb_add--;
            }

            for(; nb_add > 0; nb_add--) delay *= 2;
        } else
            for(; nb_add < 0; nb_add++) delay /= 2;

        if(delay < MIN_SIMULATION_DELAY)
            delay = MIN_SIMULATION_DELAY;
        else if(delay > MAX_SIMULATION_DELAY)
            delay = MAX_SIMULATION_DELAY;

        set_simulation_delay(shared_data, delay);
    }
}

SDL_Color get_color(float ratio) {
    SDL_Color color = {0, 0, 0, 255};

    if(ratio < 0.2f) {
        color.r = 255;
        color.g = (int) round(255.0 * ratio / 0.2);
    } else if(ratio < 0.4f) {
        color.r = (int) round(255.0 * (1 - (ratio - 0.2) / 0.2));
        color.g = 255;
    } else if(ratio < 0.6f) {
        color.g = 255;
        color.b = (int) round(255.0 * (ratio - 0.4) / 0.2);
    } else if(ratio < 0.8f) {
        color.g = (int) round(255.0 * (1 - (ratio - 0.6) / 0.2));
        color.b = 255;
    } else {
        color.r = (int) round(255.0 * (ratio - 0.8) / 0.2);
        color.b = 255;
    }

    return color;
}

bool bar_drawing(Render* render, Shared_data shared_data, int window_w, int window_h, bool colorized) {
    int save_array_len = get_save_array_len(shared_data);
    int array_len = get_array_len(shared_data);
    int cursor = get_cursor(shared_data);
    SDL_Rect bar = {0, 0, 0, 0};

    int w_unit = window_w / array_len;
    w_unit = w_unit > 0 ? w_unit : 1;

    int w_offset = (window_w - w_unit * array_len) / 2;
    w_offset = w_offset >= 0 ? w_offset : 0;

    for(int i = 0; i < array_len; i++) {
        int val = get_array_value(shared_data, i);
        float ratio = ((float) val) / ((float) save_array_len);
        SDL_Color color;

        if(colorized)
            color = i == cursor ? WHITE_COLOR : get_color(ratio);
        else
            color = i == cursor ? RED_COLOR : WHITE_COLOR;

        bar.w = w_unit;
        bar.h = (int) round(((float) window_h) * ratio);
        bar.x = w_offset + w_unit * i;
        bar.y = window_h - bar.h;

        if(!draw_rect(render, &bar, color))
            return false;
    }

    return true;
}

bool dot_drawing(Render* render, Shared_data shared_data, int window_w, int window_h, bool colorized) {
    int save_array_len = get_save_array_len(shared_data);
    int array_len = get_array_len(shared_data);
    int cursor = get_cursor(shared_data);
    SDL_Rect bar = {0, 0, 0, 0};

    int w_unit = window_w / array_len;
    w_unit = w_unit > 0 ? w_unit : 1;

    int w_offset = (window_w - w_unit * array_len) / 2;
    w_offset = w_offset >= 0 ? w_offset : 0;

    for(int i = 0; i < array_len; i++) {
        int val = get_array_value(shared_data, i);
        float ratio = ((float) val) / ((float) save_array_len);
        SDL_Color color;

        if(colorized)
            color = i == cursor ? WHITE_COLOR : get_color(ratio);
        else
            color = i == cursor ? RED_COLOR : WHITE_COLOR;

        bar.w = w_unit;
        bar.h = w_unit;
        bar.x = w_offset + w_unit * i;
        bar.y = (int) round(((float) window_h) - ((float) window_h) * ratio);

        if(!draw_rect(render, &bar, color))
            return false;
    }

    return true;
}

bool circle_drawing(Render* render, Shared_data shared_data, int window_w, int window_h, bool colorized) {
    int save_array_len = get_save_array_len(shared_data);
    int array_len = get_array_len(shared_data);
    int cursor = get_cursor(shared_data);
    
    int center_x = window_w / 2, center_y = window_h / 2;
    int radius = window_w < window_h ? window_w / 2 : window_h / 2;
    int nb_triangles = array_len < CIRCLE_MIN_TRIANGLES ? CIRCLE_MIN_TRIANGLES : array_len;

    float step = ((float) (M_PI * 2)) / ((float) nb_triangles);
    for(int i = 0; i < nb_triangles; i++) {
        int current = (int) floor(((float) i) / ((float) nb_triangles) * ((float) array_len));
        int val = get_array_value(shared_data, current);
        float ratio = ((float) val) / ((float) save_array_len);
        SDL_Color color = current == cursor ? WHITE_COLOR : get_color(ratio);

        float angle1 = step * ((float) i) - M_PI / 2;
        float angle2 = step * ((float) (i + 1)) - M_PI / 2;
        int x1 = radius * cosf(angle1);
        int y1 = radius * sinf(angle1);
        int x2 = radius * cosf(angle2);
        int y2 = radius * sinf(angle2);

        SDL_Vertex center = {{center_x, center_y}, color, {1, 1}};
        SDL_Vertex a = {{center_x + x1, center_y + y1}, color, {1, 1}};
        SDL_Vertex b = {{center_x + x2, center_y + y2}, color, {1, 1}};

        if(!draw_triangle(render, center, a, b, color))
            return false;
    }

    return true;
}

bool snail_drawing(Render* render, Shared_data shared_data, int window_w, int window_h, bool colorized) {
    int save_array_len = get_save_array_len(shared_data);
    int array_len = get_array_len(shared_data);
    int cursor = get_cursor(shared_data);

    int center_x = window_w / 2, center_y = window_h / 2;
    int radius = window_w < window_h ? window_w / 2 : window_h / 2;

    float step = ((float) (M_PI * 2)) / ((float) array_len);
    for(int i = 0; i < array_len; i++) {
        int val = get_array_value(shared_data, i);
        float ratio = ((float) val) / ((float) save_array_len);
        SDL_Color color;

        if(colorized)
            color = i == cursor ? WHITE_COLOR : get_color(ratio);
        else
            color = i == cursor ? RED_COLOR : WHITE_COLOR;

        float ratio_radius = ((float) radius) * ratio;
        if(ratio_radius <= 0) ratio_radius = 1;

        float angle1 = step * ((float) i) - SNAIL_DRAWING_CORRECTION_ANGLE;
        float angle2 = step * ((float) (i + 1)) + SNAIL_DRAWING_CORRECTION_ANGLE;
        int x1 = ratio_radius * cosf(angle1);
        int y1 = ratio_radius * sinf(angle1);
        int x2 = ratio_radius * cosf(angle2);
        int y2 = ratio_radius * sinf(angle2);

        SDL_Vertex center = {{center_x, center_y}, color, {1, 1}};
        SDL_Vertex a = {{center_x + x1, center_y + y1}, color, {1, 1}};
        SDL_Vertex b = {{center_x + x2, center_y + y2}, color, {1, 1}};

        if(!draw_triangle(render, center, a, b, color))
            return false;
    }

    return true;
}

bool draw_one_info(Render* render, char* text, int x, int y, int w, int h, SDL_Color color) {
    SDL_Rect text_rect = {x, y, w, h};
    return draw_text(render, text, &text_rect, color);
}

bool draw_one_func_info(Render* render, char* title, char* content, int line, SDL_Color color) {
    sprintf(TEXT_BUFFER, "%s: %s", title, content);
    return draw_one_info(render, TEXT_BUFFER, 0, TEXT_H * line, TEXT_LETTER_W * strlen(TEXT_BUFFER), TEXT_H, color);
}

bool draw_func_info(Render* render, Shared_data shared_data, bool colorized) {
    SDL_Color color = colorized ? WHITE_COLOR : ORANGE_COLOR;
    int min, sec, ms, corrected_min, corrected_sec, corrected_ms, corrected_us;
    decompose_ms(get_time(shared_data), &min, &sec, &ms);
    decompose_us(get_corrected_time(shared_data), &corrected_min, &corrected_sec, &corrected_ms, &corrected_us);

    if(!draw_one_func_info(render, NAME_TEXT, get_sort_algo_name(shared_data), 0, color))
        return false;

    if(!draw_one_func_info(render, COMPLEXITY_TEXT, get_sort_algo_complexity(shared_data), 1, color))
        return false;

    sprintf(TIME_BUFFER, "%02dmin  %02ds  %03dms", min, sec, ms);
    if(!draw_one_func_info(render, TIME_TEXT, TIME_BUFFER, 2, color))
        return false;

    sprintf(TIME_BUFFER, "%02dmin  %02ds  %03dms %03dµs", corrected_min, corrected_sec, corrected_ms, corrected_us);
    return draw_one_func_info(render, CORRECTED_TIME_TEXT, TIME_BUFFER, 3, color);
}

bool draw_program_info(Render* render, Shared_data shared_data, int fps, bool paused, int window_w, int window_h, bool colorized) {
    SDL_Color color = colorized ? WHITE_COLOR : ORANGE_COLOR;
    bool first_line_taken = false;

    sprintf(TEXT_BUFFER, "%d/%d", get_sort_algo_index(shared_data) + 1, SORT_ALGORITHMS_LEN);
    int w = TEXT_LETTER_W * strlen(TEXT_BUFFER);
    if(!draw_one_info(render, TEXT_BUFFER, window_w - w, 0, w, TEXT_H, color))
        return false;

    if(is_shuffling(shared_data)) {
        first_line_taken = true;
        w = TEXT_LETTER_W * SHUFFLE_TEXT_LEN;
        if(!draw_one_info(render, SHUFFLE_TEXT, window_w - w, TEXT_H, w, TEXT_H, color))
            return false;
    } else if(is_validating(shared_data)) {
        first_line_taken = true;
        w = TEXT_LETTER_W * VALIDATE_TEXT_LEN;
        if(!draw_one_info(render, VALIDATE_TEXT, window_w - w, TEXT_H, w, TEXT_H, color))
            return false;
    }

    if(paused) {
        w = TEXT_LETTER_W * PAUSE_TEXT_LEN;
        if(!draw_one_info(render, PAUSE_TEXT, window_w - w, TEXT_H * (1 + first_line_taken), w, TEXT_H, color))
            return false;
    }

    sprintf(TEXT_BUFFER, "%u LPS", get_lps(shared_data));
    if(!draw_one_info(render, TEXT_BUFFER, 0, window_h - TEXT_H * 2, TEXT_LETTER_W * strlen(TEXT_BUFFER), TEXT_H, color))
        return false;

    sprintf(TEXT_BUFFER, "%d FPS", fps);
    return draw_one_info(render, TEXT_BUFFER, 0, window_h - TEXT_H, TEXT_LETTER_W * strlen(TEXT_BUFFER), TEXT_H, color);
}

bool run_display(Render* render, Shared_data shared_data, short drawing_mode, bool show_info, bool colorized) {
    long fps = 0;
    int ww, wh;
    bool paused = is_paused(shared_data);
    Draw_array_function draw_array;

    if(drawing_mode == DOT_DRAWING)
        draw_array = &dot_drawing;
    else if(drawing_mode == CIRCLE_DRAWING) {
        draw_array = &circle_drawing;
        colorized = true;
    } else if(drawing_mode == SNAIL_DRAWING)
        draw_array = &snail_drawing;
    else
        draw_array = &bar_drawing;

    get_window_size(render, &ww, &wh);

    while(!has_quitted(shared_data)) {
        long fps_start_time = us_time();

        if(!fill_background(render, BLACK_COLOR) || !draw_array(render, shared_data, ww, wh, colorized))
            return false;

        if(show_info && (!draw_func_info(render, shared_data, colorized) || !draw_program_info(render, shared_data, fps, paused, ww, wh, colorized)))
            return false;

        refresh(render);
        handle_render_events(render, shared_data, &paused, &show_info);
        usleep(SEC_US / render->framerate);

        long time_diff = us_time() - fps_start_time;
        fps = time_diff > 0 ? SEC_US / time_diff : 0;
    }

    return true;
}
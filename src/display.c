#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "sort.h"
#include "render.h"
#include "utils.h"
#include "display.h"

#define MIN_DELAY 0
#define MAX_DELAY SEC_US
#define DELAY_STEP 10000

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

        if(delay < MIN_DELAY)
            delay = MIN_DELAY;
        else if(delay > MAX_DELAY)
            delay = MAX_DELAY;

        set_simulation_delay(shared_data, delay);
    }
}

bool draw_array(Render* render, Shared_data shared_data, int window_w, int window_h) {
    int save_array_len = get_save_array_len(shared_data);
    int array_len = get_array_len(shared_data);
    int cursor = get_cursor(shared_data);
    SDL_Rect bar = {0, 0, 0, 0};

    int w_unit = window_w / array_len;
    w_unit = w_unit > 0 ? w_unit : 1;

    int w_offset = (window_w - w_unit * array_len) / 2;
    w_offset = w_offset >= 0 ? w_offset : 0;

    for(int i = 0; i < array_len; i++) {
        SDL_Color color = i == cursor ? RED_COLOR : WHITE_COLOR;
        int val = get_array_value(shared_data, i);

        bar.w = w_unit;
        bar.h = window_h * (((float) val) / ((float) save_array_len));
        bar.x = w_offset + w_unit * i;
        bar.y = window_h - bar.h;

        if(!draw_rect(render, &bar, color))
            return false;
    }

    return true;
}

bool draw_one_info(Render* render, char* text, int x, int y, int w, int h) {
    SDL_Rect text_rect = {x, y, w, h};
    return draw_text(render, text, &text_rect, ORANGE_COLOR);
}

bool draw_one_func_info(Render* render, char* title, char* content, int line) {
    sprintf(TEXT_BUFFER, "%s: %s", title, content);
    return draw_one_info(render, TEXT_BUFFER, 0, TEXT_H * line, TEXT_LETTER_W * strlen(TEXT_BUFFER), TEXT_H);
}

bool draw_func_info(Render* render, Shared_data shared_data) {
    int min, sec, ms, corrected_min, corrected_sec, corrected_ms, corrected_us;
    decompose_ms(get_time(shared_data), &min, &sec, &ms);
    decompose_us(get_corrected_time(shared_data), &corrected_min, &corrected_sec, &corrected_ms, &corrected_us);

    if(!draw_one_func_info(render, NAME_TEXT, get_sort_algo_name(shared_data), 0))
        return false;

    if(!draw_one_func_info(render, COMPLEXITY_TEXT, get_sort_algo_complexity(shared_data), 1))
        return false;

    sprintf(TIME_BUFFER, "%02dmin  %02ds  %03dms", min, sec, ms);
    if(!draw_one_func_info(render, TIME_TEXT, TIME_BUFFER, 2))
        return false;

    sprintf(TIME_BUFFER, "%02dmin  %02ds  %03dms %03dµs", corrected_min, corrected_sec, corrected_ms, corrected_us);
    return draw_one_func_info(render, CORRECTED_TIME_TEXT, TIME_BUFFER, 3);
}

bool draw_program_info(Render* render, Shared_data shared_data, int fps, bool paused, int window_w, int window_h) {
    bool first_line_taken = false;
    if(is_shuffling(shared_data)) {
        first_line_taken = true;
        int w = TEXT_LETTER_W * SHUFFLE_TEXT_LEN;
        if(!draw_one_info(render, SHUFFLE_TEXT, window_w - w, 0, w, TEXT_H))
            return false;
    } else if(is_validating(shared_data)) {
        first_line_taken = true;
        int w = TEXT_LETTER_W * VALIDATE_TEXT_LEN;
        if(!draw_one_info(render, VALIDATE_TEXT, window_w - w, 0, w, TEXT_H))
            return false;
    }

    if(paused) {
        int w = TEXT_LETTER_W * PAUSE_TEXT_LEN;
        if(!draw_one_info(render, PAUSE_TEXT, window_w - w, TEXT_H * first_line_taken, w, TEXT_H))
            return false;
    }

    sprintf(TEXT_BUFFER, "%u LPS", get_lps(shared_data));
    if(!draw_one_info(render, TEXT_BUFFER, 0, window_h - TEXT_H * 2, TEXT_LETTER_W * strlen(TEXT_BUFFER), TEXT_H))
        return false;

    sprintf(TEXT_BUFFER, "%d FPS", fps);
    return draw_one_info(render, TEXT_BUFFER, 0, window_h - TEXT_H, TEXT_LETTER_W * strlen(TEXT_BUFFER), TEXT_H);
}

bool run_display(Render* render, Shared_data shared_data, bool show_info) {
    long fps = 0;
    int ww, wh;
    bool paused = is_paused(shared_data);

    get_window_size(render, &ww, &wh);

    while(!has_quitted(shared_data)) {
        long fps_start_time = us_time();

        if(!fill_background(render, BLACK_COLOR) || !draw_array(render, shared_data, ww, wh))
            return false;

        if(show_info && (!draw_func_info(render, shared_data) || !draw_program_info(render, shared_data, fps, paused, ww, wh)))
            return false;

        refresh(render);
        handle_render_events(render, shared_data, &paused, &show_info);
        usleep(SEC_US / render->framerate);

        long time_diff = us_time() - fps_start_time;
        fps = time_diff > 0 ? SEC_US / time_diff : 0;
    }

    return true;
}
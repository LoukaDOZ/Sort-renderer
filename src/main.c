#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include "render.h"
#include "sort.h"
#include "bubble.h"

#define HALF_SEC 500000

#define ARGS_SUCCESS 0
#define ARGS_FAILURE 1
#define ARGS_NO_RUN 2

#define NO_TIME -1
#define SHOW_TEXTS SDL_TRUE
#define HIDE_TEXTS SDL_FALSE

#define NAME_TEXT "Sort"
#define COMPLEXITY_TEXT "Average complexity"
#define TIME_TEXT "Time"
#define CORRECTED_TIME_TEXT "Corrected time"

#define SHUFFLE_TEXT "Shuffling"
#define SHUFFLE_TEXT_LEN 9

#define PAUSE_TEXT "Paused"
#define PAUSE_TEXT_LEN 6

#define TEXT_LETTER_W 8
#define TEXT_H 15

#define TEXT_BUFFER_SIZE 200
char TEXT_BUFFER[TEXT_BUFFER_SIZE];

#define TIME_BUFFER_SIZE 100
char TIME_BUFFER[TIME_BUFFER_SIZE];

typedef struct Args {
    int w, h, framerate, fullscreen;
} Args;

typedef struct Thread_arg {
    pthread_mutex_t* mutex;
    Sort_info* info;
    Render* render;
} Thread_arg;

SDL_bool HAS_QUITTED = SDL_FALSE;
SDL_bool HAS_PAUSED = SDL_FALSE;
SDL_bool SHOW_INFO = SDL_TRUE;
int NEXT_SORT = 0;


SDL_bool inverse_bool(SDL_bool b) {
    return (b + 1) % 2;
}

void handle_render_events(Render* render) {
    int tmp_framerate = render->framerate;
    handle_events(render);

    if(do_quit(render) || was_pressed(render, KEY_Q) > 0)
        HAS_QUITTED = SDL_TRUE;

    if(was_pressed(render, KEY_I) % 2 > 0)
        SHOW_INFO = inverse_bool(SHOW_INFO);

    NEXT_SORT += was_pressed(render, KEY_ARROW_RIGHT);
    NEXT_SORT -= was_pressed(render, KEY_ARROW_LEFT);

    if(was_pressed(render, KEY_P) % 2 > 0)
        HAS_PAUSED = inverse_bool(HAS_PAUSED);

    int nb_up = was_pressed(render, MOUSE_WHEEL_UP) + was_pressed(render, KEY_ARROW_UP);
    int nb_down = was_pressed(render, MOUSE_WHEEL_DOWN) + was_pressed(render, KEY_ARROW_DOWN);

    if(nb_up > 0 && HAS_PAUSED && render->framerate <= 0)
        HAS_PAUSED = SDL_FALSE;

    tmp_framerate += nb_up;
    tmp_framerate -= nb_down;
    
    if(tmp_framerate <= 0) {
        HAS_PAUSED = SDL_TRUE;
        tmp_framerate = 0;
    }

    render->framerate = tmp_framerate;
}

int max(int a, int b) {
    return a > b ? a : b;
}

long ms_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

long us_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void tick(Render* render) {
    long step = render->framerate > 0 ? 1000000 / render->framerate : 0;
    long start = us_time();
    long limit = start + step;

    handle_render_events(render);
    while(us_time() < limit && !HAS_QUITTED && NEXT_SORT == 0) {
        handle_render_events(render);
        step = render->framerate > 0 ? 1000000 / render->framerate : 0;
        limit = start + step;
    }
}

SDL_bool draw_array(Render* render, Sort_info* info, int window_w, int window_h) {
    int barw = max(floor(window_w / info->array_len), 1);
    SDL_Rect bar = {0, 0, barw, 0};

    for(int i = 0; i < info->array_len && !HAS_QUITTED && NEXT_SORT == 0; i++) {
        SDL_Color color = i == info->cursor ? RED_COLOR : WHITE_COLOR;
        float ratio = ((float) info->array[i]) / ((float) info->array_len);
        bar.h = floor(window_h * ratio);
        bar.x = bar.w * i;
        bar.y = window_h - bar.h;

        if(!draw_rect(render, &bar, color))
            return SDL_FALSE;
    }

    return SDL_TRUE;
}

SDL_bool draw_one_info(Render* render, char* text, int x, int y, int w, int h) {
    SDL_Rect text_rect = {x, y, w, h};
    return draw_text(render, text, &text_rect, ORANGE_COLOR);
}

SDL_bool draw_one_func_info(Render* render, char* title, char* content, int line) {
    sprintf(TEXT_BUFFER, "%s: %s", title, content);
    return draw_one_info(render, TEXT_BUFFER, 0, TEXT_H * line, TEXT_LETTER_W * strlen(TEXT_BUFFER), TEXT_H);
}

SDL_bool draw_func_info(Render* render, const Sort_function* func, long time_ms, long corrected_time_us) {
    int ms = 0, sec = 0, min = 0;
    int corrected_us = 0, corrected_ms = 0, corrected_sec = 0, corrected_min = 0;

    if(time_ms != NO_TIME) {
        ms = time_ms % 1000;
        sec = (time_ms / 1000) % 60;
        min = time_ms / 60000;
    }

    if(corrected_time_us != NO_TIME) {
        corrected_us = corrected_time_us % 1000;
        corrected_ms = (corrected_time_us / 1000) % 1000;
        corrected_sec = (corrected_time_us / 1000000) % 60;
        corrected_min = corrected_time_us / 60000000;
    }

    if(!draw_one_func_info(render, NAME_TEXT, func->name, 0))
        return SDL_FALSE;

    if(!draw_one_func_info(render, COMPLEXITY_TEXT, func->complexity, 1))
        return SDL_FALSE;

    sprintf(TIME_BUFFER, "%02dmin  %02ds  %03dms", min, sec, ms);
    if(!draw_one_func_info(render, TIME_TEXT, TIME_BUFFER, 2))
        return SDL_FALSE;

    sprintf(TIME_BUFFER, "%02dmin  %02ds  %03dms %03dµs", corrected_min, corrected_sec, corrected_ms, corrected_us);
    return draw_one_func_info(render, CORRECTED_TIME_TEXT, TIME_BUFFER, 3);
}

SDL_bool draw_program_info(Render* render, double lps, int ips, SDL_bool is_shuffling, int window_w, int window_h) {
    if(is_shuffling) {
        int w = TEXT_LETTER_W * SHUFFLE_TEXT_LEN;
        if(!draw_one_info(render, SHUFFLE_TEXT, window_w - w, 0, w, TEXT_H))
            return SDL_FALSE;
    }

    if(HAS_PAUSED && !draw_one_info(render, PAUSE_TEXT, 0, window_h - TEXT_H * 3, TEXT_LETTER_W * PAUSE_TEXT_LEN, TEXT_H))
        return SDL_FALSE;

    sprintf(TEXT_BUFFER, "%0.2lf LPS", lps);
    if(!draw_one_info(render, TEXT_BUFFER, 0, window_h - TEXT_H * 2, TEXT_LETTER_W * strlen(TEXT_BUFFER), TEXT_H))
            return SDL_FALSE;

    sprintf(TEXT_BUFFER, "%d IPS", ips);
    return draw_one_info(render, TEXT_BUFFER, 0, window_h - TEXT_H, TEXT_LETTER_W * strlen(TEXT_BUFFER), TEXT_H);
}

SDL_bool draw_display(Render* render, Sort_info* info, const Sort_function* func, long time_ms, long corrected_time_us, double lps, SDL_bool is_shuffling) {
    int ww, wh;
    get_window_size(render, &ww, &wh);

    if(!fill_background(render, BLACK_COLOR) || !draw_array(render, info, ww, wh))
        return SDL_FALSE;

    if(SHOW_INFO && (!draw_func_info(render, func, time_ms, corrected_time_us) 
            || !draw_program_info(render, lps, (int) lps, is_shuffling, ww, wh)))
        return SDL_FALSE;

    refresh(render);
    return SDL_TRUE;
}

SDL_bool shuffle(Render* render, Sort_info* info, const Sort_function* func) {
    double loop_time = 0;

    for(int i = 0; i < info->array_len && !HAS_QUITTED && NEXT_SORT == 0; i++) {
        long loop_start_time = us_time();
        tick(render);

        if(!HAS_PAUSED) {
            int rand_i = rand() % info->array_len;
            int tmp = info->array[rand_i];

            info->array[rand_i] = info->array[i];
            info->array[i] = tmp;
            info->cursor = i;

            loop_time = ((double) (us_time() - loop_start_time));
        } else
            i--;

        if(!draw_display(render, info, func, NO_TIME, NO_TIME, 1. / (loop_time / 1000000), SDL_TRUE))
            return SDL_FALSE;
    }

    if(!HAS_QUITTED && NEXT_SORT == 0) sleep(1);
    return SDL_TRUE;
}

SDL_bool sort(Render* render, Sort_info* info, const Sort_function* func) {
    short state = SORT_SUCCESS;
    long start_time = ms_time();
    long running_time = 0;
    long total_time = 0;
    long corrected_total_time = 0;
    double loop_time = 0;

    while(!HAS_QUITTED && NEXT_SORT == 0 && state == SORT_SUCCESS) {
        long loop_start_time = us_time();
        tick(render);

        if(!HAS_PAUSED) {
            long corrected_start_time = us_time();
            state = func->sort(info);
            corrected_total_time += us_time() - corrected_start_time;
            running_time = ms_time() - start_time;
            loop_time = ((double) (us_time() - loop_start_time));
        } else {
            total_time += running_time;
            start_time = ms_time();
            running_time = 0;
        }

        if(state == SORT_FAILURE || !draw_display(render, info, func, total_time + running_time, corrected_total_time, 1. / (loop_time / 1000000), SDL_FALSE))
            return SDL_FALSE;
    }

    if(!HAS_QUITTED && NEXT_SORT == 0) sleep(1);
    return SDL_TRUE;
}

SDL_bool run(Render* render)  {
    Sort_info* info = init_sort_info(500);
    SDL_bool res_state = SDL_TRUE;
    int sort_func_i = 0;

    while(!HAS_QUITTED) {
        const Sort_function* func = &(SORT_FUNCTIONS[sort_func_i]);

        if(!shuffle(render, info, func) || func->init(info) != SORT_SUCCESS) {
            res_state = SDL_FALSE;
            break;
        }

        if(!HAS_QUITTED && !sort(render, info, func)) {
            func->free(info);
            res_state = SDL_FALSE;
            break;
        }

        func->free(info);
        if(NEXT_SORT == 0) sort_func_i++;
        sort_func_i = abs((sort_func_i + NEXT_SORT) % SORT_FUNCTIONS_LEN);
        NEXT_SORT = 0;
    }

    free_sort_info(info);
    return res_state;
}

void init_args(Args* args) {
    args->w = 500;
    args->h = 500;
    args->framerate = 60;
    args->fullscreen = 0;
}

void free_args(Args* args) {
    //ignore
}

int get_args(Args* args, int argc, char** argv) {
    init_args(args);
    
    return ARGS_SUCCESS;
}

int main(int argc, char** argv) {
    //Handle args
    Args args;

    int r = get_args(&args, argc, argv);
    if(r == ARGS_NO_RUN)
        return EXIT_SUCCESS;
    else if(r == ARGS_FAILURE)
        return EXIT_FAILURE;

    // Start program
    int w, h;

    Render* render = init_render(args.w, args.h, args.framerate, args.fullscreen);
    if(render == NULL) {
        fprintf(stderr, "An error occur when inititalizing render : %s\n", render_error());
        return EXIT_FAILURE;
    }

    free_args(&args);
    show_window(render);
    
    SDL_bool run_state = run(render);
    if(!run_state)
        fprintf(stderr, "An error occur when rendering : %s\n", render_error());

    destroy_render(render);
    return run_state ? EXIT_SUCCESS : EXIT_FAILURE;
}
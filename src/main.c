#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
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
#define DURATION_TEXT "Duration"
#define CORECTED_DURATION_TEXT "Duration (framerate corrected)"

#define TEXT_LETTER_W 8
#define TEXT_H 15

#define TEXT_BUFFER_SIZE 100
char TEXT_BUFFER[TEXT_BUFFER_SIZE];

#define TIME_BUFFER_SIZE 20
char TIME_BUFFER[TIME_BUFFER_SIZE];

typedef struct Args {
    int w, h, framerate, fullscreen;
} Args;

SDL_bool HAS_QUITTED = SDL_FALSE;

void handle_render_events(Render* render) {
    handle_events(render);

    if(do_quit(render) || was_pressed(render, KEY_Q) > 0)
        HAS_QUITTED = SDL_TRUE;
}

int max(int a, int b) {
    return a > b ? a : b;
}

long ms_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

SDL_bool draw_array(Render* render, Sort_info* info, int window_w, int window_h) {
    int barw = max(floor(window_w / info->array_len), 1);
    SDL_Rect bar = {0, 0, barw, 0};

    for(int i = 0; i < info->array_len && !HAS_QUITTED; i++) {
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

SDL_bool draw_one_text_info(Render* render, char* title, char* content, int line) {
    sprintf(TEXT_BUFFER, "%s: %s", title, content);
    SDL_Rect text_rect = {0, TEXT_H * line, TEXT_LETTER_W * strlen(TEXT_BUFFER), TEXT_H};
    
    return draw_text(render, TEXT_BUFFER, &text_rect, ORANGE_COLOR);
}

SDL_bool draw_text_info(Render* render, Sort_info* info, long start_time, int window_w, int window_h) {
    int ms = 0;
    int sec = 0;
    int min = 0;

    if(start_time != NO_TIME) {
        struct timeval current_time;
        gettimeofday(&current_time, NULL);

        long time_diff = ms_time() - start_time;
        ms = time_diff % 1000;
        sec = (time_diff / 1000) % 60;
        min = time_diff / 60000;
    }

    if(!draw_one_text_info(render, NAME_TEXT, info->name, 0))
        return SDL_FALSE;

    if(!draw_one_text_info(render, COMPLEXITY_TEXT, info->complexity, 1))
        return SDL_FALSE;

    sprintf(TIME_BUFFER, "%02d:%02d.%03d", min, sec, ms);
    if(!draw_one_text_info(render, DURATION_TEXT, TIME_BUFFER, 2))
        return SDL_FALSE;

    sprintf(TIME_BUFFER, "%02d:%02d", min, sec);
    return draw_one_text_info(render, CORECTED_DURATION_TEXT, TIME_BUFFER, 3);

    return SDL_TRUE;
}

SDL_bool draw(Render* render, Sort_info* info, long start_time, SDL_bool show_texts) {
    int ww, wh;
    get_window_size(render, &ww, &wh);

    if(!fill_background(render, BLACK_COLOR) || !draw_array(render, info, ww, wh))
        return SDL_FALSE;
    
    if(show_texts && !draw_text_info(render, info, start_time, ww, wh))
        return SDL_FALSE;

    refresh(render);
    return SDL_TRUE;
}

SDL_bool run_sort(Render* render, Sort_info* info, const Sort_function* sort_func) {
    if(!draw(render, info, NO_TIME, SHOW_TEXTS))
        return SDL_FALSE;

    long start_time = ms_time();
    while(!HAS_QUITTED) {
        tick(render);
        handle_render_events(render);
        int state = sort_func->sort(info);

        if(state == SORT_FAILURE)
            return SDL_FALSE;

        if(state == SORT_FINISHED)
            break;

        if(!draw(render, info, start_time, SHOW_TEXTS))
            return SDL_FALSE;

        refresh(render);
    }

    return SDL_TRUE;
}

SDL_bool shuffle(Render* render, Sort_info* info) {
    int save_cursor = info->cursor;

    for(int i = 0; i < info->array_len && !HAS_QUITTED; i++) {
        tick(render);
        handle_render_events(render);

        int randi = rand() % info->array_len;
        int tmp = info->array[i];

        info->array[i] = info->array[randi];
        info->array[randi] = tmp;
        info->cursor = i;

        if(!draw(render, info, NO_TIME, SHOW_TEXTS))
            return SDL_FALSE;
    }

    info->cursor = save_cursor;
    return SDL_TRUE;
}

SDL_bool run(Render* render) {
    int i = 0;

    while(!HAS_QUITTED) {
        Sort_info* info = init_sort(50);
        const Sort_function* sort_func = &(SORT_FUNCTIONS[i]);

        if(sort_func->init(info) != SORT_SUCCESS) {
            free_sort_info(info);
            return SDL_FALSE;
        }

        shuffle(render, info);

        if(HAS_QUITTED) {
            sort_func->free(info);
            free_sort_info(info);
            break;
        }

        usleep(HALF_SEC);
        SDL_bool state = run_sort(render, info, sort_func);
        
        sort_func->free(info);
        free_sort_info(info);

        if(!state)
            return SDL_FALSE;

        if(HAS_QUITTED)
            break;
        
        usleep(HALF_SEC);

        i++;
        if(i >= SORT_FUNCTIONS_LEN)
            i = 0;
    }

    return SDL_TRUE;
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

typedef struct Thread_arg {
    pthread_mutex_t* mutex;
    Sort_info* info;
} Thread_arg;

void* aa(void* args) {
    Thread_arg* targs = (Thread_arg*) args;
    Sort_info* info = targs->info;
    long return_value = 0;
    int i = 0;

    while(!HAS_QUITTED) {
        const Sort_function* sort_func = &(SORT_FUNCTIONS[i]);

        pthread_mutex_lock(targs->mutex); // != 0
        if(sort_func->init(info) != SORT_SUCCESS) {
            pthread_mutex_unlock(targs->mutex);
            free_sort_info(info);
            return_value = 1;
            break;
        }
        pthread_mutex_unlock(targs->mutex); // != 0

        pthread_mutex_lock(targs->mutex); // != 0
        if(HAS_QUITTED) {
            pthread_mutex_unlock(targs->mutex);
            sort_func->free(info);
            free_sort_info(info);
            break;
        }
        pthread_mutex_unlock(targs->mutex); // != 0

        usleep(HALF_SEC);
        SDL_bool state = SDL_TRUE;
        for(int i = 0; i < info->array_len && !HAS_QUITTED; i++) {
            pthread_mutex_lock(targs->mutex); // != 0
            info->cursor = i;
            pthread_mutex_unlock(targs->mutex); // != 0
            usleep(100000);
        }
        
        pthread_mutex_lock(targs->mutex); // != 0
        sort_func->free(info);
        pthread_mutex_unlock(targs->mutex); // != 0

        if(!state) {
            return_value = 1;
            break;
        }

        if(HAS_QUITTED)
            break;
        
        usleep(HALF_SEC);

        i++;
        if(i >= SORT_FUNCTIONS_LEN)
            i = 0;
    }

    printf("EXIT %d\n", HAS_QUITTED);
    pthread_exit((void*) return_value);
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

    Sort_info* info = init_sort(50);
    pthread_t pid;
    int ret = 0;
    pthread_mutex_t mutex; // TODO HAS_QUIT MUTEX
    pthread_mutex_init(&mutex, NULL); // != 0
    Thread_arg ta = {&mutex, info};
    pthread_create(&pid, NULL, &aa, &ta);

    while(!HAS_QUITTED) {
        tick(render);
        fill_background(render, BLACK_COLOR);
        handle_render_events(render);
        pthread_mutex_lock(&mutex); // != 0
        draw_array(render, info, 500, 500);
        pthread_mutex_unlock(&mutex); // != 0
        refresh(render);
    }
    printf("WAIT\n");
    pthread_join(pid, (void**) &ret); // != 0
    pthread_mutex_destroy(&mutex);
    free_sort_info(info);
    printf("STOP %d\n", ret);

    /*if(!run(render)) {
        fprintf(stderr, "An error occur when rendering : %s\n", render_error());
        destroy_render(render);
        return EXIT_FAILURE;
    }*/

    destroy_render(render);
    return EXIT_SUCCESS;
}
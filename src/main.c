#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include "sort.h"
#include "render.h"
#include "display.h"
#include "simulation.h"
#include "shared.h"
#include "utils.h"

#define ARGS_SUCCESS 0
#define ARGS_FAILURE 1
#define ARGS_NO_RUN 2

typedef struct Args {
    int w, h, framerate;
    bool fullscreen;
} Args;

void init_args(Args* args) {
    args->w = 500;
    args->h = 500;
    args->framerate = 60;
    args->fullscreen = false;
}

void free_args(Args* args) {
    //ignore
}

int get_args(Args* args, int argc, char** argv) {
    init_args(args);
    
    return ARGS_SUCCESS;
}

void* run_thread(void* args) {
    usleep(SEC_US);
    long res_state = run_simulation((Shared_data) args) ? 0 : 1;
    pthread_exit((void*) res_state);
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

    Shared_data shared_data = create_shared_data(250);
    if(shared_data == NULL) {
        fprintf(stderr, "An error occur when inititalizing simulation data : %s\n", render_error());
        destroy_render(render);
        return EXIT_FAILURE;
    }

    pthread_t tid;
    int thread_res = 0;
    if(pthread_create(&tid, NULL, &run_thread, shared_data) != 0) {
        perror("An error occur when starting simulation ");
        free_shared_data(shared_data);
        destroy_render(render);
        return EXIT_FAILURE;
    }
    
    bool display_state = run_display(render, shared_data);
    //pthread_kill(tid, SIGKILL);
    pthread_join(tid, (void**) &thread_res);

    if(!display_state)
        fprintf(stderr, "An error occur when running simulation (display) : %s\n", render_error());

    if(thread_res != 0)
        fprintf(stderr, "An error occur when running simulation (simulation) : %s\n", render_error());

    free_shared_data(shared_data);
    destroy_render(render);
    return display_state && thread_res == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
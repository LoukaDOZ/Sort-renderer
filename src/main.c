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

#define MIN_WIDTH 2
#define MIN_HEIGHT 2
#define MIN_FRAMERATE 1
#define MAX_FRAMERATE SEC_US
#define MIN_LOOPRATE 1
#define MAX_LOOPRATE SEC_US
#define MIN_ARRAY_SIZE 3

#define ARGS_SUCCESS 0
#define ARGS_FAILURE 1
#define ARGS_NO_RUN 2

#define DRAWING_MODE_ARG_BAR "BAR"
#define DRAWING_MODE_ARG_DOT "DOT"
#define DRAWING_MODE_ARG_CIRCLE "CIRCLE"
#define DRAWING_MODE_ARG_SNAIL "SNAIL"

typedef struct Args {
    int w, h, array_size, framerate, looprate, sort, array_size_changed_index, drawing_mode;
    bool fullscreen, show_info, validate, same_shuffle, auto_change_sort, colorized;
} Args;

void init_args(Args* args) {
    args->w = 500;
    args->h = 500;
    args->array_size = args->w;
    args->framerate = 60;
    args->looprate = 500;
    args->sort = 0;
    args->fullscreen = false;
    args->show_info = true;
    args->validate = true;
    args->same_shuffle = false;
    args->auto_change_sort = true;
    args->colorized = false;
    args->array_size_changed_index = -1;
    args->drawing_mode = BAR_DRAWING;
}

bool has_next_arg(int argc, char** argv, int i) {
    return i >= argc - 1 ? false : true;
}

bool get_enum_arg(int argc, char** argv, int i, char** enum_values, int enum_values_len, int* value_index) {
    if(!has_next_arg(argc, argv, i)) {
        fprintf(stderr, "Missing value for '%s'\n", argv[i]);
        return false;
    }

    char* arg = argv[i + 1];
    for(int i = 0; i < enum_values_len; i++) {
        if(strcasecmp(arg, enum_values[i]) == 0) {
            *value_index = i;
            return true;
        }
    }
    
    fprintf(stderr, "Invalid value for '%s' : %s\n", argv[i], arg);
    return false;
}

bool get_int_arg(int argc, char** argv, int i, int* value) {
    if(!has_next_arg(argc, argv, i)) {
        fprintf(stderr, "Missing value for '%s'\n", argv[i]);
        return false;
    }

    char* arg = argv[i + 1];
    char* end;
    *value = strtol(arg, &end, 10);

    if((*value == 0 && (errno != 0 || arg == end)) || strlen(end) > 0) {
        fprintf(stderr, "Invalid value for '%s' : %s\n", argv[i], arg);
        return false;
    }
    
    return true;
}

bool validate_int(char* arg, int value, int min, int max) {
    if(value < min || value > max) {
        fprintf(stderr, "Invalid value for '%s' : %d\n", arg, value);
        return false;
    }

    return true;
}

bool validate_int_min(char* arg, int value, int min) {
    if(value < min) {
        fprintf(stderr, "Invalid value for '%s' : %d\n", arg, value);
        return false;
    }

    return true;
}

void print_simulation_controls(void) {
    printf("\nIn simulation controls :\n");
    printf("\tQ\t\t\t\tQuit\n");
    printf("\tP\t\t\t\tPause\n");
    printf("\tR\t\t\t\tRestart current sort\n");
    printf("\tI\t\t\t\tToggle information\n");
    printf("\tLeft arrow\t\t\tPrevious sort\n");
    printf("\tRight arrow\t\t\tNext sort\n");
    printf("\tUp arrow, mouse scrool up\tIncrease simulation looprate\n");
    printf("\tDown arrow, mouse scrool down\tDecrease simulation looprate\n");
}

int get_args(Args* args, int argc, char** argv) {
    init_args(args);

    for(int i = 1; i < argc; i++) {
        char* arg = argv[i];

        if(strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
            printf("Usage: %s <options>\n", argv[0]);
            printf("\nOptions :\n");
            printf("\t-h, --help\t\t\tDisplay this help message\n");
            printf("\t-w, --width <int>\t\tScreen width in pixels (%d < width) (default: 500px)\n", MIN_WIDTH);
            printf("\t-h, --height <int>\t\tScreen height in pixels (%d < height) (default: 500px)\n", MIN_HEIGHT);
            printf("\t-r, --framerate <int>\t\tDisplay max frames per seconds (%d < framerate < %ld) (default: 60/s)\n", MIN_FRAMERATE, MAX_FRAMERATE);
            printf("\t-l, --looprate <int>\t\tSimulation max loops per seconds (%d < looprate < %ld) (default: 500/s)\n", MIN_LOOPRATE, MAX_LOOPRATE);
            printf("\t-a, --array-size <int>\t\tArray size (%d < array size < width) (default: screen width)\n", MIN_ARRAY_SIZE);
            printf("\t-s, --sort <int>\t\tStarting sort index modulo SORT_ALGORITHMS_LEN (%d) (default: 0)\n", SORT_ALGORITHMS_LEN);
            printf("\t-d, --drawing-mode <str>\tWay to render the array : 'BAR', 'DOT', 'CIRCLE' or 'SNAIL' (default: 'BAR') ('CIRCLE' is always colorized)\n");
            printf("\t-f, --fullscreen\t\tSet fullscreen\n");
            printf("\t-c, --colorized\t\t\tColorize display\n");
            printf("\t-n, --manual-next-sort\t\tDisable launching next sort automatically\n");
            printf("\t-m, --same-shuffle\t\tSet the output array after shuffling to always be the same\n");
            printf("\t-v, --no-validation\t\tDisable validating the array is properly sorted after execution of an algorithm\n");
            printf("\t-i, --no-info\t\t\tDisable information messages\n");

            print_simulation_controls();
            return ARGS_NO_RUN;
        }

        if(strcmp(arg, "--fullscreen") == 0 || strcmp(arg, "-f") == 0) {
            args->fullscreen = true;
        } else if(strcmp(arg, "--no-info") == 0 || strcmp(arg, "-i") == 0) {
            args->show_info = false;
        } else if(strcmp(arg, "--no-validation") == 0 || strcmp(arg, "-v") == 0) {
            args->validate = false;
        } else if(strcmp(arg, "--manual-next-sort") == 0 || strcmp(arg, "-n") == 0) {
            args->auto_change_sort = false;
        } else if(strcmp(arg, "--same-shuffle") == 0 || strcmp(arg, "-m") == 0) {
            args->same_shuffle = true;
        } else if(strcmp(arg, "--colorized") == 0 || strcmp(arg, "-c") == 0) {
            args->colorized = true;
        } else if(strcmp(arg, "--width") == 0 || strcmp(arg, "-w") == 0) {
            if(!get_int_arg(argc, argv, i, &(args->w)) || !validate_int_min(arg, args->w, MIN_WIDTH))
                return ARGS_FAILURE;
            
            i++;
        } else if(strcmp(arg, "--height") == 0 || strcmp(arg, "-e") == 0) {
            if(!get_int_arg(argc, argv, i, &(args->h)) || !validate_int_min(arg, args->h, MIN_HEIGHT))
                return ARGS_FAILURE;
            
            i++;
        } else if(strcmp(arg, "--framerate") == 0 || strcmp(arg, "-r") == 0) {
            if(!get_int_arg(argc, argv, i, &(args->framerate)) || !validate_int(arg, args->framerate, MIN_FRAMERATE, MAX_FRAMERATE))
                return ARGS_FAILURE;
            
            i++;
        } else if(strcmp(arg, "--looprate") == 0 || strcmp(arg, "-l") == 0) {
            if(!get_int_arg(argc, argv, i, &(args->looprate)) || !validate_int(arg, args->looprate, MIN_LOOPRATE, MAX_LOOPRATE))
                return ARGS_FAILURE;

            i++;
        } else if(strcmp(arg, "--array-size") == 0 || strcmp(arg, "-a") == 0) {
            if(!get_int_arg(argc, argv, i, &(args->array_size)))
                return ARGS_FAILURE;

            args->array_size_changed_index = i;
            i++;
        } else if(strcmp(arg, "--sort") == 0 || strcmp(arg, "-s") == 0) {
            if(!get_int_arg(argc, argv, i, &(args->sort)))
                return ARGS_FAILURE;

            i++;
        } else if(strcmp(arg, "--draw-type") == 0 || strcmp(arg, "-d") == 0) {
            char* enum_values[4] = {DRAWING_MODE_ARG_BAR, DRAWING_MODE_ARG_DOT, DRAWING_MODE_ARG_CIRCLE, DRAWING_MODE_ARG_SNAIL};
            int value_index;

            if(!get_enum_arg(argc, argv, i, enum_values, 4, &value_index))
                return ARGS_FAILURE;

            switch(value_index) {
                case 1:
                    args->drawing_mode = DOT_DRAWING;
                    break;

                case 2:
                    args->drawing_mode = CIRCLE_DRAWING;
                    break;

                case 3:
                    args->drawing_mode = SNAIL_DRAWING;
                    break;
                
                default:
                case 0:
                    args->drawing_mode = BAR_DRAWING;
                    break;
            }

            i++;
        } else {
            fprintf(stderr, "Invalid option : '%s', use --help for help\n", arg);
            return ARGS_FAILURE;
        }
    }
    
    return ARGS_SUCCESS;
}

void* run_thread(void* args) {
    void** args_array = (void**) args;
    long res_state = run_simulation((Shared_data) args_array[0], *((bool*) args_array[1]), *((bool*) args_array[2]),  *((bool*) args_array[3]));
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
    print_simulation_controls();

    Render* render = init_render(args.w, args.h, args.framerate, args.fullscreen);
    if(render == NULL) {
        fprintf(stderr, "An error occur when inititalizing render : %s\n", render_error());
        return EXIT_FAILURE;
    }

    show_window(render);
    get_window_size(render, &w, &h);

    bool array_size_changed = false;
    if(args.array_size_changed_index >= 0) {
        if(!validate_int(argv[args.array_size_changed_index], args.array_size, MIN_ARRAY_SIZE, w)) {
            destroy_render(render);
            return EXIT_FAILURE;
        }
        
        array_size_changed = true;
    }

    Shared_data shared_data = create_shared_data(array_size_changed ? args.array_size : w, SEC_US / args.looprate, args.sort);
    if(shared_data == NULL) {
        fprintf(stderr, "An error occur when inititalizing simulation data : %s\n", render_error());
        destroy_render(render);
        return EXIT_FAILURE;
    }

    pthread_t tid;
    int thread_res = 0;
    void* targs[] = { (void*) shared_data, (void*) (&(args.validate)), (void*) (&(args.same_shuffle)), (void*) (&(args.auto_change_sort)) };
    if(pthread_create(&tid, NULL, &run_thread, targs) != 0) {
        perror("An error occur when starting simulation ");
        free_shared_data(shared_data);
        destroy_render(render);
        return EXIT_FAILURE;
    }
    
    bool display_state = run_display(render, shared_data, args.drawing_mode, args.show_info, args.colorized);
    pthread_join(tid, (void**) &thread_res);

    if(!display_state)
        fprintf(stderr, "An error occur when running simulation (display) : %s\n", render_error());

    if(thread_res == ALGORITHM_USER_FAILURE)
        fprintf(stderr, "An error occur when running simulation (simulation) : SORT_FAILURE returned\n");
    else if(thread_res == ALGORITHM_FAILURE)
        fprintf(stderr, "An error occur when running simulation (simulation) : the array was not sorted properly\n");
    else if(thread_res != SIMULATION_SUCCESS)
        fprintf(stderr, "An error occur when running simulation (simulation)\n");

    free_shared_data(shared_data);
    destroy_render(render);
    return display_state && thread_res == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include "sort.h"
#include "functions.h"
#include "shared.h"
#include "api.h"
#include "utils.h"
#include "simulation.h"

#define SLEEP_TIME 1

void free_simulation_data(Data* data) {
    free(data->_private);
    free(data->array);
    free(data);
}

Data* create_simulation_data(Shared_data shared_data) {
    Data* data = (Data*) malloc(sizeof(Data));
    if(data == NULL)
        return NULL;

    data->array_len = get_save_array_len(shared_data);
    data->array = (int*) malloc(sizeof(int) * data->array_len);
    if(data->array == NULL) {
        free(data);
        return NULL;
    }

    Private* private = (Private*) malloc(sizeof(Private));
    if(private == NULL) {
        free(data->array);
        free(data);
        return NULL;
    }

    private->shared_data = shared_data;
    private->sort_algo_index = 0;
    private->simulation_start_time = 0;
    private->simulation_time = 0;
    private->loop_start_time = 0;
    private->corrected_time = 0;
    private->run = true;
    private->has_quitted = false;

    data->cursor = 0;
    data->_private = private;
    return data;
}

void reset(Data* data) {
    Shared_data shared_data = (Shared_data) ((Private*) data->_private)->shared_data;
    Sort_info* info = lock_and_get_info(shared_data);

    reset_sort_info(info);
    data->array_len = info->array_len;
    data->cursor = info->cursor;

    for(int i = 0; i < info->array_len; i++)
        data->array[i] = info->array[i];

    unlock_info(shared_data);
}

void init_simulation(Data* data) {
    Private* private = (Private*) data->_private;
    set_time(private->shared_data, 0);
    set_corrected_time(private->shared_data, 0);

    private->corrected_time = 0;
    private->simulation_time = 0;
    private->simulation_start_time = ms_time();
    private->loop_start_time = us_time();
}

void shuffle(Data* data) {
    Private* private = (Private*) data->_private;
    Shared_data shared_data = (Shared_data) private->shared_data;
    set_is_shuffling(shared_data, true);

    for(int i = 0; i < data->array_len && private->run; i++) {
        int rand_i = rand() % data->array_len;
        int current = get_array_value(shared_data, i);
        int random = get_array_value(shared_data, rand_i);

        data->array[i] = random;
        data->array[rand_i] = current;
        data->cursor = i;

        tick(data);
        set_time(shared_data, 0);
        set_corrected_time(shared_data, 0);
    }

    set_is_shuffling(shared_data, false);
}

bool run_simulation(Shared_data shared_data) {
    short simulation_res = SORT_SUCCESS;
    bool state = true;

    Data* data = create_simulation_data(shared_data);
    if(data == NULL)
        return false;

    Private* private = (Private*) data->_private;
    while(!private->has_quitted) {
        private->sort_algo_index = get_sort_algo_index(shared_data);
        private->run = true;

        reset(data);
        shuffle(data);

        if(!private->run)  
            continue;

        sleep(SLEEP_TIME);
        init_simulation(data);
        simulation_res = SORT_ALGORITHMS[private->sort_algo_index].function(data);
        
        if(simulation_res == SORT_FAILURE) {
            state = false;
            break;
        }

        if(private->run) {
            sleep(SLEEP_TIME);
            set_sort_algo_index(shared_data, 1);
        }
    }

    free_simulation_data(data);
    return state;
}
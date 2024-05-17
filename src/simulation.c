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
#define TOTAL_TIME SEC_US

const short SIMULATION_SUCCESS = 0;
const short SIMULATION_FAILURE = 1;
const short ALGORITHM_FAILURE = 2;
const short ALGORITHM_USER_FAILURE = 3;

typedef void (Shuffle_function)(Data* data, int* shuffle_array, int i);

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

    Sort_info* info = lock_and_get_info(shared_data);
    for(int i = 0; i < info->array_len; i++)
        data->array[i] = info->array[i];
    unlock_info(shared_data);

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

void init_simulation(Data* data) {
    Private* private = (Private*) data->_private;
    set_time(private->shared_data, 0);
    set_corrected_time(private->shared_data, 0);

    private->corrected_time = 0;
    private->simulation_time = 0;
    private->simulation_start_time = ms_time();
    private->loop_start_time = us_time();
}

bool validate(Data* data) {
    Private* private = (Private*) data->_private;
    Shared_data shared_data = (Shared_data) private->shared_data;

    unsigned long start_time = get_time(shared_data);
    unsigned long start_corrected_time = get_corrected_time(shared_data);
    unsigned long start_delay = get_simulation_delay(shared_data);
    unsigned long forced_delay = TOTAL_TIME / data->array_len;

    set_is_validating(shared_data, true);

    for(int i = 0; i < data->array_len && private->run; i++) {
        if(i > 0 && data->array[i - 1] > data->array[i])
            return false;

        if(get_simulation_delay(shared_data) != forced_delay)
            set_simulation_delay(shared_data, forced_delay);

        data->cursor = i;
        tick(data);
        set_time(private->shared_data, start_time);
        set_corrected_time(private->shared_data, start_corrected_time);
    }

    set_is_validating(shared_data, false);
    set_simulation_delay(shared_data, start_delay);
    return true;
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

int* create_shuffle_array(Data* data) {
    Shared_data shared_data = (Shared_data) ((Private*) data->_private)->shared_data;

    int* shuffle_array = (int*) malloc(sizeof(int) * data->array_len);
    if(shuffle_array == NULL)
        return NULL;

    for(int i = 0; i < data->array_len; i++)
        shuffle_array[i] = get_array_value(shared_data, i);

    for(int i = 0; i < data->array_len; i++) {
        int rand_i = rand() % data->array_len;

        int tmp = shuffle_array[i];
        shuffle_array[i] = shuffle_array[rand_i];
        shuffle_array[rand_i] = tmp;
    }

    return shuffle_array;
}

void classic_shuffle(Data* data, int* shuffle_array, int i) {
    int rand_i = rand() % data->array_len;
    swap(data, i, rand_i);
}

void same_shuffle(Data* data, int* shuffle_array, int i) {
    data->array[i] = shuffle_array[i];
}

void shuffle(Data* data, int* shuffle_array, Shuffle_function shuffle_function) {
    Private* private = (Private*) data->_private;
    Shared_data shared_data = (Shared_data) private->shared_data;

    unsigned long start_delay = get_simulation_delay(shared_data);
    unsigned long forced_delay = TOTAL_TIME / data->array_len;

    set_is_shuffling(shared_data, true);

    for(int i = 0; i < data->array_len && private->run; i++) {
        shuffle_function(data, shuffle_array, i);
        data->cursor = i;

        if(get_simulation_delay(shared_data) != forced_delay)
            set_simulation_delay(shared_data, forced_delay);

        tick(data);
        set_time(shared_data, 0);
        set_corrected_time(shared_data, 0);
    }

    set_is_shuffling(shared_data, false);
    set_simulation_delay(shared_data, start_delay);
}

short run_simulation(Shared_data shared_data, bool run_validation, bool use_same_shuffle, bool auto_change_sort) {
    short simulation_res = SORT_SUCCESS;
    short state = SIMULATION_SUCCESS;

    Shuffle_function* shuffle_function = classic_shuffle;
    int* shuffle_array = NULL;

    Data* data = create_simulation_data(shared_data);
    if(data == NULL) {
        set_has_quitted(shared_data, true);
        return SIMULATION_FAILURE;
    }

    if(use_same_shuffle) {
        shuffle_array = create_shuffle_array(data);
        shuffle_function = same_shuffle;

        if(shuffle_array == NULL) {
            set_has_quitted(shared_data, true);
            free_simulation_data(data);
            return SIMULATION_FAILURE;
        }
    }

    Private* private = (Private*) data->_private;
    while(!private->has_quitted) {
        private->sort_algo_index = get_sort_algo_index(shared_data);
        private->run = true;

        reset(data);
        shuffle(data, shuffle_array, shuffle_function);

        if(!private->run)  
            continue;

        sleep(SLEEP_TIME);
        init_simulation(data);
        simulation_res = SORT_ALGORITHMS[private->sort_algo_index].function(data);
        
        if(simulation_res == SORT_FAILURE) {
            state = ALGORITHM_USER_FAILURE;
            break;
        }

        if(private->run && run_validation && !validate(data)) {
            state = ALGORITHM_FAILURE;
            break;
        }

        if(private->run) {
            sleep(SLEEP_TIME);

            if(auto_change_sort)
                set_sort_algo_index(shared_data, 1);
            else {
                unsigned long time = get_time(shared_data);
                unsigned long corrected_time = get_corrected_time(shared_data);

                while(private->sort_algo_index == get_sort_algo_index(shared_data) && !private->has_quitted) {
                    tick(data);
                    set_time(shared_data, time);
                    set_corrected_time(shared_data, corrected_time);
                    set_lps(shared_data, 0);
                }
            }
        }
    }

    set_has_quitted(shared_data, true);
    free_simulation_data(data);
    free(shuffle_array);
    return state;
}
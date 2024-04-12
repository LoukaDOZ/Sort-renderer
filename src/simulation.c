#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "sort.h"
#include "shared.h"
#include "utils.h"
#include "simulation.h"

#define SUCCESS 0
#define FINSIHED 1
#define FAILURE 2
#define CHANGED 3

short reset(Shared_data shared_data, unsigned int* current_sort_index, unsigned int* start_time, unsigned int* saved_time, unsigned long* corrected_time) {
    *start_time = 0;
    *saved_time = 0;
    *corrected_time = 0;
    *current_sort_index = get_sort_function_index(shared_data);
    set_cursor(shared_data, 0);
    return reset_info(shared_data) == SORT_SUCCESS && sort_function_init(shared_data) == SORT_SUCCESS ? SUCCESS : FAILURE;
}

short shuffling_step(Shared_data shared_data) {
    int array_len = get_array_len(shared_data);
    int cursor = get_cursor(shared_data);

    if(cursor >= array_len)
        return FINSIHED;

    int rand_i = rand() % array_len;
    int tmp = get_array_value(shared_data, rand_i);
    int current = get_array_value(shared_data, cursor);

    set_array_value(shared_data, rand_i, current);
    set_array_value(shared_data, cursor, tmp);
    set_cursor(shared_data, cursor + 1);
    
    return SUCCESS;
}

short sorting_step(Shared_data shared_data, unsigned long* corrected_time) {
    unsigned long corrected_start_time = us_time();
    short state = sort_function_sort(shared_data);
    *corrected_time += us_time() - corrected_start_time;

    if(state == SORT_SUCCESS)
        return SUCCESS;

    if(state == SORT_FINISHED)
        return FINSIHED;
    
    return FAILURE;
}

bool run_simulation(Shared_data shared_data) {
    unsigned int current_sort_index, start_time, diff_time, saved_time;
    unsigned long corrected_time;
    double double_sec_us = ((double) SEC_US);
    
    bool shuffling = true;
    reset(shared_data, &current_sort_index, &start_time, &saved_time, &corrected_time);

    while(!has_quitted(shared_data)) {
        long loop_start_time = us_time();

        if(!is_paused(shared_data)) {
            if(current_sort_index != get_sort_function_index(shared_data)) {
                sort_function_free(shared_data);
                shuffling = true;
                set_is_shuffling(shared_data, true);
                reset(shared_data, &current_sort_index, &start_time, &saved_time, &corrected_time);
            }

            if(shuffling) {
                short state = shuffling_step(shared_data);

                if(state != SUCCESS) {
                    if(state == FAILURE)
                        return false;

                    sleep(1);
                    set_is_shuffling(shared_data, false);
                    shuffling = false;
                    start_time = ms_time();
                    corrected_time = 0;
                }
            } else {
                short state = sorting_step(shared_data, &corrected_time);
                diff_time = ms_time() - start_time;

                if(state != SUCCESS) {
                    sort_function_free(shared_data);

                    if(state == FAILURE)
                        return false;

                    sleep(1);
                    shuffling = true;
                    set_is_shuffling(shared_data, true);
                    set_sort_function(shared_data, 1);
                    reset(shared_data, &current_sort_index, &start_time, &saved_time, &corrected_time);
                }
            }
        } else {
            saved_time += diff_time;
            start_time = ms_time();
            diff_time = 0;
        }

        set_time(shared_data, saved_time + diff_time);
        set_corrected_time(shared_data, corrected_time);

        usleep(get_simulation_delay(shared_data));
        long diff = us_time() - loop_start_time;
        set_lps(shared_data, diff <= SEC_US ? double_sec_us / ((double) diff) : SEC_US);
    }

    return true;
}
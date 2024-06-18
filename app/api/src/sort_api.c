#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include "shared.h"
#include "simulation.h"
#include "utils.h"
#include "sort_api.h"

const short SORT_SUCCESS = 0;
const short SORT_FAILURE = 1;

void copy_data(Data* data, Shared_data shared_data) {
    Sort_info* info = lock_and_get_info(shared_data);

    info->cursor = data->cursor;
    info->array_len = data->array_len;
    
    for(int i = 0; i < data->array_len; i++)
        info->array[i] = data->array[i];

    unlock_info(shared_data);
}

bool run(Data* data) {
    return ((Private*) data->_private)->run;
}

void tick(Data* data) {
    Private* private = (Private*) data->_private;
    long corrected_duration = us_time() - private->loop_start_time;
    long time = ms_time() - private->simulation_start_time;

    private->corrected_time += corrected_duration;
    private->has_quitted = has_quitted(private->shared_data);

    while(is_paused(private->shared_data)) {
        private->has_quitted = has_quitted(private->shared_data);
        private->simulation_time += time;
        private->simulation_start_time = ms_time();
        time = 0;

        if(private->has_quitted) {
            private->run = false;
            return;
        }
    }

    int tmp = get_sort_algo_index(private->shared_data);
    private->run = private->sort_algo_index == tmp && !private->has_quitted && !has_restarted(private->shared_data);
    copy_data(data, private->shared_data);

    usleep(get_simulation_delay(private->shared_data));

    long loop_duration = us_time() - private->loop_start_time;
    set_lps(private->shared_data, ((long) roundf(((double) SEC_US) / ((double) loop_duration))) );
    set_corrected_time(private->shared_data, private->corrected_time);
    set_time(private->shared_data, private->simulation_time + time);
    private->loop_start_time = us_time();
}

void swap(Data* data, int a, int b) {
    int tmp = data->array[a];
    data->array[a] = data->array[b];
    data->array[b] = tmp;
}
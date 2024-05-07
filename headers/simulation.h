#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdbool.h> // Fix unknown bool
#include "shared.h" // Fix unknown Shared_data

extern const short SIMULATION_SUCCESS;
extern const short SIMULATION_FAILURE;
extern const short ALGORITHM_FAILURE;

typedef struct Private {
    Shared_data shared_data;
    unsigned int sort_algo_index;
    long simulation_start_time, simulation_time, loop_start_time, corrected_time;
    bool run, has_quitted;
} Private;

short run_simulation(Shared_data shared_data);

#endif
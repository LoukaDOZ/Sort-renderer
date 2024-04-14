#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include "sort.h"
#include "render.h"
#include "shared.h"

#define NB_SHARED_POINTER 10
#define INFO_SHARED_POINTER 0
#define HAS_QUITTED_SHARED_POINTER 1
#define IS_PAUSED_SHARED_POINTER 2
#define IS_SHUFFLING_SHARED_POINTER 3
#define TIME_SHARED_POINTER 4
#define CORRECTED_TIME_SHARED_POINTER 5
#define LPS_SHARED_POINTER 6
#define SIMULATION_DELAY_SHARED_POINTER 7
#define SORT_FUNCTION_SHARED_POINTER 8
#define NEXT_SORT_FUNCTION_SHARED_POINTER 9

typedef struct Shared_pointer {
    void* pointer;
    pthread_mutex_t mutex;
} Shared_pointer;



void free_shared_pointer(Shared_pointer* ptr) {
    pthread_mutex_destroy(&(ptr->mutex));
    free(ptr);
}

void free_v_shared_pointer(Shared_pointer* ptr) {
    free(ptr->pointer);
    free_shared_pointer(ptr);
}

void free_shared_data(Shared_data data) {
    for(int i = 1; i < NB_SHARED_POINTER; i++)
        free_v_shared_pointer(data[i]);

    free_sort_info((Sort_info*) (data[INFO_SHARED_POINTER]->pointer));
    free_shared_pointer(data[INFO_SHARED_POINTER]);
    free(data);
}

Shared_pointer* create_shared_pointer() {
    Shared_pointer* ptr = (Shared_pointer*) malloc(sizeof(Shared_pointer));
    if(ptr == NULL)
        return NULL;
    
    if(pthread_mutex_init(&(ptr->mutex), NULL) != 0) {
        free(ptr);
        return NULL;
    }

    return ptr;
}

Shared_pointer* create_v_shared_pointer(int v_size) {
    Shared_pointer* ptr = create_shared_pointer();
    if(ptr == NULL)
        return NULL;
    
    ptr->pointer = (void*) malloc(v_size);
    if(ptr->pointer == NULL) {
        free_shared_pointer(ptr);
        return NULL;
    }

    return ptr;
}

void free_incomplete_data(Shared_data data, int limit) {
    for(int i = 1; i < limit; i++)
        free_v_shared_pointer(data[i]);
    
    free_shared_pointer(data[INFO_SHARED_POINTER]);
    free(data);
}

Shared_data create_shared_data(int array_size, int simulation_delay) {
    Shared_data data = (Shared_data) malloc(sizeof(Shared_pointer*) * NB_SHARED_POINTER);
    if(data == NULL)
        return NULL;

    data[INFO_SHARED_POINTER] = create_shared_pointer();
    if(data[INFO_SHARED_POINTER] == NULL) {
        free(data);
        return NULL;
    }

    int limit = 1;
    data[HAS_QUITTED_SHARED_POINTER] = create_v_shared_pointer(sizeof(bool));
    if(data[HAS_QUITTED_SHARED_POINTER] == NULL) {
        free_incomplete_data(data, limit);
        return NULL;
    }
    limit++;

    data[IS_PAUSED_SHARED_POINTER] = create_v_shared_pointer(sizeof(bool));
    if(data[IS_PAUSED_SHARED_POINTER] == NULL) {
        free_incomplete_data(data, limit);
        return NULL;
    }
    limit++;

    data[IS_SHUFFLING_SHARED_POINTER] = create_v_shared_pointer(sizeof(bool));
    if(data[IS_SHUFFLING_SHARED_POINTER] == NULL) {
        free_incomplete_data(data, limit);
        return NULL;
    }
    limit++;

    data[TIME_SHARED_POINTER] = create_v_shared_pointer(sizeof(unsigned int));
    if(data[TIME_SHARED_POINTER] == NULL) {
        free_incomplete_data(data, limit);
        return NULL;
    }
    limit++;

    data[CORRECTED_TIME_SHARED_POINTER] = create_v_shared_pointer(sizeof(unsigned long));
    if(data[CORRECTED_TIME_SHARED_POINTER] == NULL) {
        free_incomplete_data(data, limit);
        return NULL;
    }
    limit++;

    data[LPS_SHARED_POINTER] = create_v_shared_pointer(sizeof(unsigned int));
    if(data[LPS_SHARED_POINTER] == NULL) {
        free_incomplete_data(data, limit);
        return NULL;
    }
    limit++;

    data[SIMULATION_DELAY_SHARED_POINTER] = create_v_shared_pointer(sizeof(unsigned long));
    if(data[SIMULATION_DELAY_SHARED_POINTER] == NULL) {
        free_incomplete_data(data, limit);
        return NULL;
    }
    limit++;

    data[SORT_FUNCTION_SHARED_POINTER] = create_v_shared_pointer(sizeof(unsigned int));
    if(data[SORT_FUNCTION_SHARED_POINTER] == NULL) {
        free_incomplete_data(data, limit);
        return NULL;
    }
    limit++;

    data[NEXT_SORT_FUNCTION_SHARED_POINTER] = create_v_shared_pointer(sizeof(int));
    if(data[NEXT_SORT_FUNCTION_SHARED_POINTER] == NULL) {
        free_incomplete_data(data, limit);
        return NULL;
    }
    limit++;

    Sort_info* info = init_sort_info(array_size);
    if(info == NULL) {
        free_incomplete_data(data, limit);
        return NULL;
    }

    data[INFO_SHARED_POINTER]->pointer = (void*) info;
    *((bool*) data[HAS_QUITTED_SHARED_POINTER]->pointer) = false;
    *((bool*) data[IS_PAUSED_SHARED_POINTER]->pointer) = false;
    *((bool*) data[IS_SHUFFLING_SHARED_POINTER]->pointer) = false;
    *((unsigned int*) data[TIME_SHARED_POINTER]->pointer) = 0;
    *((unsigned long*) data[CORRECTED_TIME_SHARED_POINTER]->pointer) = 0;
    *((unsigned int*) data[LPS_SHARED_POINTER]->pointer) = 0;
    *((unsigned long*) data[SIMULATION_DELAY_SHARED_POINTER]->pointer) = simulation_delay;
    *((unsigned int*) data[SORT_FUNCTION_SHARED_POINTER]->pointer) = 0;
    *((int*) data[NEXT_SORT_FUNCTION_SHARED_POINTER]->pointer) = 0;
    return data;
}

void lock_shared_pointer(Shared_pointer* ptr) {
    pthread_mutex_lock(&(ptr->mutex));
}

void unlock_shared_pointer(Shared_pointer* ptr) {
    pthread_mutex_unlock(&(ptr->mutex));
}

void set_has_quitted(Shared_data data, bool quit) {
    lock_shared_pointer(data[HAS_QUITTED_SHARED_POINTER]);
    *((bool*) data[HAS_QUITTED_SHARED_POINTER]->pointer) = quit;
    unlock_shared_pointer(data[HAS_QUITTED_SHARED_POINTER]);
}

bool has_quitted(Shared_data data) {
    lock_shared_pointer(data[HAS_QUITTED_SHARED_POINTER]);
    bool quit = *((bool*) data[HAS_QUITTED_SHARED_POINTER]->pointer);
    unlock_shared_pointer(data[HAS_QUITTED_SHARED_POINTER]);
    return quit;
}

void set_is_paused(Shared_data data, bool pause) {
    lock_shared_pointer(data[IS_PAUSED_SHARED_POINTER]);
    *((bool*) data[IS_PAUSED_SHARED_POINTER]->pointer) = pause;
    unlock_shared_pointer(data[IS_PAUSED_SHARED_POINTER]);
}

bool is_paused(Shared_data data) {
    lock_shared_pointer(data[IS_PAUSED_SHARED_POINTER]);
    bool pause = *((bool*) data[IS_PAUSED_SHARED_POINTER]->pointer);
    unlock_shared_pointer(data[IS_PAUSED_SHARED_POINTER]);
    return pause;
}

void set_is_shuffling(Shared_data data, bool shuffling) {
    lock_shared_pointer(data[IS_SHUFFLING_SHARED_POINTER]);
    *((bool*) data[IS_SHUFFLING_SHARED_POINTER]->pointer) = shuffling;
    unlock_shared_pointer(data[IS_SHUFFLING_SHARED_POINTER]);
}

bool is_shuffling(Shared_data data) {
    lock_shared_pointer(data[IS_SHUFFLING_SHARED_POINTER]);
    bool shuffling = *((bool*) data[IS_SHUFFLING_SHARED_POINTER]->pointer);
    unlock_shared_pointer(data[IS_SHUFFLING_SHARED_POINTER]);
    return shuffling;
}

void set_time(Shared_data data, unsigned int time) {
    lock_shared_pointer(data[TIME_SHARED_POINTER]);
    *((unsigned int*) data[TIME_SHARED_POINTER]->pointer) = time;
    unlock_shared_pointer(data[TIME_SHARED_POINTER]);
}

unsigned int get_time(Shared_data data) {
    lock_shared_pointer(data[TIME_SHARED_POINTER]);
    unsigned int time = *((unsigned int*) data[TIME_SHARED_POINTER]->pointer);
    unlock_shared_pointer(data[TIME_SHARED_POINTER]);
    return time;
}

void set_corrected_time(Shared_data data, unsigned long time) {
    lock_shared_pointer(data[CORRECTED_TIME_SHARED_POINTER]);
    *((unsigned long*) data[CORRECTED_TIME_SHARED_POINTER]->pointer) = time;
    unlock_shared_pointer(data[CORRECTED_TIME_SHARED_POINTER]);
}

unsigned long get_corrected_time(Shared_data data) {
    lock_shared_pointer(data[CORRECTED_TIME_SHARED_POINTER]);
    unsigned long time = *((unsigned long*) data[CORRECTED_TIME_SHARED_POINTER]->pointer);
    unlock_shared_pointer(data[CORRECTED_TIME_SHARED_POINTER]);
    return time;
}

void set_lps(Shared_data data, unsigned int lps) {
    lock_shared_pointer(data[LPS_SHARED_POINTER]);
    *((unsigned int*) data[LPS_SHARED_POINTER]->pointer) = lps;
    unlock_shared_pointer(data[LPS_SHARED_POINTER]);
}

unsigned int get_lps(Shared_data data) {
    lock_shared_pointer(data[LPS_SHARED_POINTER]);
    unsigned int lps = *((unsigned int*) data[LPS_SHARED_POINTER]->pointer);
    unlock_shared_pointer(data[LPS_SHARED_POINTER]);
    return lps;
}

void set_simulation_delay(Shared_data data, unsigned long delay) {
    lock_shared_pointer(data[SIMULATION_DELAY_SHARED_POINTER]);
    *((unsigned long*) data[SIMULATION_DELAY_SHARED_POINTER]->pointer) = delay;
    unlock_shared_pointer(data[SIMULATION_DELAY_SHARED_POINTER]);
}

unsigned long get_simulation_delay(Shared_data data) {
    lock_shared_pointer(data[SIMULATION_DELAY_SHARED_POINTER]);
    unsigned long delay = *((unsigned long*) data[SIMULATION_DELAY_SHARED_POINTER]->pointer);
    unlock_shared_pointer(data[SIMULATION_DELAY_SHARED_POINTER]);
    return delay;
}

void add_next_sort_shift(Shared_data data, int shift) {
    lock_shared_pointer(data[NEXT_SORT_FUNCTION_SHARED_POINTER]);
    *((int*) data[NEXT_SORT_FUNCTION_SHARED_POINTER]->pointer) += shift;
    unlock_shared_pointer(data[NEXT_SORT_FUNCTION_SHARED_POINTER]);
}

int get_next_sort_shift(Shared_data data, bool empty) {
    lock_shared_pointer(data[NEXT_SORT_FUNCTION_SHARED_POINTER]);
    int shift = *((int*) data[NEXT_SORT_FUNCTION_SHARED_POINTER]->pointer);
    if(empty) *((int*) data[NEXT_SORT_FUNCTION_SHARED_POINTER]->pointer) = 0;
    unlock_shared_pointer(data[NEXT_SORT_FUNCTION_SHARED_POINTER]);
    return shift;
}

void set_sort_function(Shared_data data, int shift) {
    lock_shared_pointer(data[SORT_FUNCTION_SHARED_POINTER]);
    unsigned int* value = (unsigned int*) data[SORT_FUNCTION_SHARED_POINTER]->pointer;
    int shifted = (((int) *value) + shift) % SORT_FUNCTIONS_LEN;
    *value = shifted < 0 ? SORT_FUNCTIONS_LEN + shifted : shifted;
    unlock_shared_pointer(data[SORT_FUNCTION_SHARED_POINTER]);
}

unsigned int get_sort_function_index(Shared_data data) {
    lock_shared_pointer(data[SORT_FUNCTION_SHARED_POINTER]);
    unsigned int function_i = *((unsigned int*) data[SORT_FUNCTION_SHARED_POINTER]->pointer);
    unlock_shared_pointer(data[SORT_FUNCTION_SHARED_POINTER]);
    return function_i;
}

const Sort_function* get_sort_function(Shared_data data) {
    lock_shared_pointer(data[SORT_FUNCTION_SHARED_POINTER]);
    unsigned int function_i = *((unsigned int*) data[SORT_FUNCTION_SHARED_POINTER]->pointer);
    unlock_shared_pointer(data[SORT_FUNCTION_SHARED_POINTER]);
    return &SORT_FUNCTIONS[function_i];
}

char* get_sort_function_name(Shared_data data) {
    lock_shared_pointer(data[SORT_FUNCTION_SHARED_POINTER]);
    unsigned int function_i = *((unsigned int*) data[SORT_FUNCTION_SHARED_POINTER]->pointer);
    unlock_shared_pointer(data[SORT_FUNCTION_SHARED_POINTER]);
    return SORT_FUNCTIONS[function_i].name;
}

char* get_sort_function_complexity(Shared_data data) {
    lock_shared_pointer(data[SORT_FUNCTION_SHARED_POINTER]);
    unsigned int function_i = *((unsigned int*) data[SORT_FUNCTION_SHARED_POINTER]->pointer);
    unlock_shared_pointer(data[SORT_FUNCTION_SHARED_POINTER]);
    return SORT_FUNCTIONS[function_i].complexity;
}

short sort_function_init(Shared_data data) {
    Sort_info* info = lock_and_get_info(data);
    const Sort_function* function = get_sort_function(data);
    short state = function->init(info);
    unlock_info(data);
    return state;
}

short sort_function_sort(Shared_data data) {
    Sort_info* info = lock_and_get_info(data);
    const Sort_function* function = get_sort_function(data);
    short state = function->sort(info);
    unlock_info(data);
    return state;
}

void sort_function_free(Shared_data data) {
    Sort_info* info = lock_and_get_info(data);
    const Sort_function* function = get_sort_function(data);
    function->free(info);
    unlock_info(data);
}

void lock_info(Shared_data data) {
    lock_shared_pointer(data[INFO_SHARED_POINTER]);
}

void unlock_info(Shared_data data) {
    unlock_shared_pointer(data[INFO_SHARED_POINTER]);
}

Sort_info* lock_and_get_info(Shared_data data) {
    lock_info(data);
    return (Sort_info*) data[INFO_SHARED_POINTER]->pointer;
}

Sort_info* get_info(Shared_data data) {
    lock_info(data);
    Sort_info* info = (Sort_info*) data[INFO_SHARED_POINTER]->pointer;
    unlock_info(data);
    return info;
}

short reset_info(Shared_data data) {
    Sort_info* info = lock_and_get_info(data);
    short state = reset_sort_info(info);
    unlock_info(data);
    return state;
}

int get_array_len(Shared_data data) {
    Sort_info* info = lock_and_get_info(data);
    int len = info->array_len;
    unlock_info(data);
    return len;
}

void set_array_value(Shared_data data, int i, int value) {
    Sort_info* info = lock_and_get_info(data);
    info->array[i] = value;
    unlock_info(data);
}

int get_array_value(Shared_data data, int i) {
    Sort_info* info = lock_and_get_info(data);
    int val = info->array[i];
    unlock_info(data);
    return val;
}

void set_cursor(Shared_data data, int cursor) {
    Sort_info* info = lock_and_get_info(data);
    info->cursor = cursor;
    unlock_info(data);
}

int get_cursor(Shared_data data) {
    Sort_info* info = lock_and_get_info(data);
    int cursor = info->cursor;
    unlock_info(data);
    return cursor;
}
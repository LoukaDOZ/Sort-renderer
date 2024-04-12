#include <stdlib.h>
#include "sort.h"

#define UNDEFINED_STR "Undefined"

const short SORT_SUCCESS = 0;
const short SORT_FAILURE = 1;
const short SORT_FINISHED = 2;

//////// Add new sorts here ////////

#include "bubble.h"
#include "insertion.h"

const int SORT_FUNCTIONS_LEN = 2;
const Sort_function SORT_FUNCTIONS[] = {
    {
        &init_insertion_sort,
        &insertion_sort,
        &free_insertion_sort
    },
    {
        &init_bubble_sort,
        &bubble_sort,
        &free_bubble_sort
    }
};

////////////////////////////////////

int* create_array(int array_size) {
    int* array = (int*) malloc(sizeof(int) * array_size);
    if(array == NULL)
        return NULL;

    for(int i = 0; i < array_size; i++)
        array[i] = i + 1;

    return array;
}

Sort_info* init_sort_info(int array_size) {
    Sort_info* info = (Sort_info*) malloc(sizeof(Sort_info));
    if(info == NULL)
        return NULL;
    
    info->array = create_array(array_size);
    if(info->array == NULL) {
        free(info);
        return NULL;
    }

    info->array_len = array_size;
    info->save_array_len = array_size;
    info->other = NULL;
    reset_sort_info(info);
    return info;
}

short reset_sort_info(Sort_info* info) {
    if(info->save_array_len != info->array_len) {
        int* array = create_array(info->save_array_len);
        if(array == NULL)
            return SORT_FAILURE;

        free(info->array);
        info->array = array;
        info->array_len = info->save_array_len;
    }

    info->cursor = 0;
    info->name = UNDEFINED_STR;
    info->complexity = UNDEFINED_STR;
    return SORT_SUCCESS;
}

void free_sort_info(Sort_info* info) {
    free(info->array);
    free(info);
}
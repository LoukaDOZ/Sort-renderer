#include <stdlib.h>
#include "sort.h"

const short SORT_SUCCESS = 0;
const short SORT_FAILURE = 1;
const short SORT_FINISHED = 2;

//////// Add new sorts here ////////

#include "bubble.h"
#include "insertion.h"

const int SORT_FUNCTIONS_LEN = 2;
const Sort_function SORT_FUNCTIONS[] = {
    {
        "Insertion sort",
        "Θ(n²)",
        &init_insertion_sort,
        &insertion_sort,
        &free_insertion_sort
    },
    {
        "Bubble sort",
        "Θ(n²)",
        &init_bubble_sort,
        &bubble_sort,
        &free_bubble_sort
    }
};

////////////////////////////////////

Sort_info* init_sort_info(int array_size) {
    Sort_info* info = (Sort_info*) malloc(sizeof(Sort_info));
    if(info == NULL)
        return NULL;
    
    info->array = (int*) calloc(array_size, sizeof(int));
    if(info->array == NULL) {
        free(info);
        return NULL;
    }

    for(int i = 0; i < array_size; i++)
        info->array[i] = i + 1;

    info->cursor = 0;
    info->array_len = array_size;
    info->other = NULL;
    return info;
}

void free_sort_info(Sort_info* info) {
    free(info->array);
    free(info);
}
#include <stdlib.h>
#include "sort.h"

const short SORT_SUCCESS = 0;
const short SORT_FAILURE = 1;
const short SORT_FINISHED = 2;

//////// Add new sorts here ////////

#include "bubble.h"
#include "insertion.h"
#include "gnome.h"
#include "shaker.h"
#include "quick.h"
#include "merge_top_down.h"
#include "merge_bottom_up.h"
#include "bongo.h"

const int SORT_FUNCTIONS_LEN = 8;
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
    },
    {
        "Gnome sort",
        "Θ(n²)",
        &init_gnome_sort,
        &gnome_sort,
        &free_gnome_sort
    },
    {
        "Shaker sort",
        "Θ(n²)",
        &init_shaker_sort,
        &shaker_sort,
        &free_shaker_sort
    },
    {
        "Quick sort",
        "Θ(n log n)",
        &init_quick_sort,
        &quick_sort,
        &free_quick_sort
    },
    {
        "Merge sort (Top-down)",
        "Θ(n log n)",
        &init_merge_top_down_sort,
        &merge_top_down_sort,
        &free_merge_top_down_sort
    },
    {
        "Merge sort (Bottom-up)",
        "Θ(n log n)",
        &init_merge_bottom_up_sort,
        &merge_bottom_up_sort,
        &free_merge_bottom_up_sort
    },
    {
        "Bongo sort",
        "Θ((n + 1)!)",
        &init_bongo_sort,
        &bongo_sort,
        &free_bongo_sort
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
    return SORT_SUCCESS;
}

void free_sort_info(Sort_info* info) {
    free(info->array);
    free(info);
}

void swap(Sort_info* info, unsigned int i, unsigned int j) {
    int tmp = info->array[i];
    info->array[i] = info->array[j];
    info->array[j] = tmp;
}
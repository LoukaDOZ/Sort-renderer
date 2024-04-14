#include <stdlib.h>
#include <stdbool.h>
#include "sort.h"
#include "shaker.h"

#define LOWER_LIMIT 0
#define UPPER_LIMIT 1

typedef struct Other_info {
    int lower_limit, upper_limit;
    bool search_max, has_swapped;
} Other_info;

short init_shaker_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) malloc(sizeof(Other_info));
    if(other_info == NULL)
        return SORT_FAILURE;

    other_info->lower_limit = 0;
    other_info->upper_limit = info->array_len - 1;
    other_info->search_max = true;
    other_info->has_swapped = false;
    info->other = (void*) other_info;
    info->cursor = 0;
    return SORT_SUCCESS;
}

short shaker_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) info->other;

    if(other_info->lower_limit == other_info->upper_limit)
        return SORT_FINISHED;

    if(other_info->search_max) {
        if(info->cursor >= other_info->upper_limit) {
            if(!other_info->has_swapped)
                return SORT_FINISHED;

            other_info->upper_limit--;
            other_info->search_max = false;
            other_info->has_swapped = false;
        } else {
            if(info->array[info->cursor] > info->array[info->cursor + 1]) {
                swap(info, info->cursor, info->cursor + 1);
                other_info->has_swapped = true;
            }

            info->cursor++;
        }
    } else {
        if(info->cursor <= other_info->lower_limit) {
            if(!other_info->has_swapped)
                return SORT_FINISHED;

            other_info->lower_limit++;
            other_info->search_max = true;
            other_info->has_swapped = false;
        } else{
            if(info->array[info->cursor] < info->array[info->cursor - 1]) {
                swap(info, info->cursor, info->cursor - 1);
                other_info->has_swapped = true;
            }

            info->cursor--;
        }
    }

    return SORT_SUCCESS;
}

void free_shaker_sort(Sort_info* info) {
    free(info->other);
}
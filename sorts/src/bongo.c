#include <stdlib.h>
#include <stdbool.h>
#include "sort.h"
#include "bongo.h"

short init_bongo_sort(Sort_info* info) {
    info->cursor = 0;
    return SORT_SUCCESS;
}

short bongo_sort(Sort_info* info) {
    int rand_i = rand() % info->array_len;
    swap(info, info->cursor, rand_i);
    
    info->cursor++;
    if(info->cursor >= info->array_len) {
        bool sorted = true;

        for(int i = 1; i < info->array_len; i++) {
            if(info->array[i - 1] > info->array[i])
                sorted = false;
        }

        if(sorted) return SORT_FINISHED;
        info->cursor = 0;
    }

    return SORT_SUCCESS;
}

void free_bongo_sort(Sort_info* info) {
    // Ignore
}
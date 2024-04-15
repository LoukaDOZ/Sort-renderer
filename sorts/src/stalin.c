#include <stdlib.h>
#include "sort.h"
#include "stalin.h"

short init_stalin_sort(Sort_info* info) {
    info->cursor = 1;
    return SORT_SUCCESS;
}

short stalin_sort(Sort_info* info) {
    if(info->array[info->cursor - 1] > info->array[info->cursor]) {
        int* new_array = (int*) malloc(sizeof(int) * info->array_len - 1);

        for(int i = 0, j = 0; i < info->array_len; i++) {
            if(i == info->cursor)
                continue;

            new_array[j] = info->array[i];
            j++;
        }

        free(info->array);
        info->array = new_array;
        info->array_len--;
    } else
        info->cursor++;
    
    if(info->cursor >= info->array_len)
        return SORT_FINISHED;

    return SORT_SUCCESS;
}

void free_stalin_sort(Sort_info* info) {
    // Ignore
}
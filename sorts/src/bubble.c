#include <stdlib.h>
#include "sort.h"
#include "bubble.h"

short init_bubble_sort(Sort_info* info) {
    int* limit = (int*) malloc(sizeof(int));
    if(limit == NULL)
        return SORT_FAILURE;

    *limit = info->array_len - 1;
    info->other = (void*) limit;
    info->cursor = 0;
    return SORT_SUCCESS;
}

short bubble_sort(Sort_info* info) {
    int* limit = (int*) info->other;

    if(*limit <= 0)
        return SORT_FINISHED;

    if(info->array[info->cursor] > info->array[info->cursor + 1])
        swap(info, info->cursor, info->cursor + 1);

    info->cursor++;
    if(info->cursor > *limit) {
        info->cursor = 0;
        (*limit)--;
    }

    return SORT_SUCCESS;
}

void free_bubble_sort(Sort_info* info) {
    free(info->other);
}
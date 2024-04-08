#include <stdlib.h>
#include "sort.h"
#include "bubble.h"

short init_bubble_sort(Sort_info* info) {
    int* limit = (int*) malloc(sizeof(int));
    if(limit == NULL)
        return SORT_FAILURE;

    *limit = info->array_len - 1;
    info->other = (void*) limit;
    info->name = "Bubble sort";
    info->complexity = "Θ(n²)";
    return SORT_SUCCESS;
}

short bubble_sort(Sort_info* info) {
    int* limit = (int*) info->other;

    if(*limit <= 0)
        return SORT_FINISHED;

    if(info->array[info->cursor] > info->array[info->cursor + 1]) {
        int tmp = info->array[info->cursor];
        info->array[info->cursor] = info->array[info->cursor + 1];
        info->array[info->cursor + 1] = tmp;
    }

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
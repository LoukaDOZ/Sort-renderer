#include <stdlib.h>
#include "sort.h"
#include "insertion.h"

short init_insertion_sort(Sort_info* info) {
    int* current = (int*) malloc(sizeof(int));
    if(current == NULL)
        return SORT_FAILURE;

    *current = 1;
    info->other = (void*) current;
    info->cursor = *current;
    info->name = "Insertion sort";
    info->complexity = "Θ(n²)";
    return SORT_SUCCESS;
}

short insertion_sort(Sort_info* info) {
    int* current = (int*) info->other;

    if(*current >= info->array_len)
        return SORT_FINISHED;

    if(info->array[info->cursor - 1] > info->array[info->cursor]) {
        int tmp = info->array[info->cursor];
        info->array[info->cursor] = info->array[info->cursor - 1];
        info->array[info->cursor - 1] = tmp;
    }

    info->cursor--;
    if(info->cursor < 1 || info->array[info->cursor - 1] <= info->array[info->cursor]) {
        (*current)++;
        info->cursor = *current;
    }

    return SORT_SUCCESS;
}

void free_insertion_sort(Sort_info* info) {
    free(info->other);
}
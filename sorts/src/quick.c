#include <stdlib.h>
#include <stdbool.h>
#include "sort.h"
#include "quick.h"

typedef struct Sub_list Sub_list;
struct Sub_list {
    Sub_list* next;
    int start, end;
};

typedef struct Other_info {
    Sub_list* heap;
    Sub_list* current;
    int pivot, last_i;
} Other_info;

void free_heap(Sub_list* heap) {
    Sub_list* current = heap;

    while(current != NULL) {
        Sub_list* tmp = current->next;
        free(current);
        current = tmp;
    }
}

bool add_to_heap(Other_info* other_info, int start, int end) {
    Sub_list* subl = (Sub_list*) malloc(sizeof(Sub_list));
    if(subl == NULL)
        return false;

    subl->next = NULL;
    subl->start = start;
    subl->end = end;

    if(other_info->heap == NULL) {
        other_info->heap = subl;
        return true;
    }

    Sub_list* tmp = other_info->current->next;
    other_info->current->next = subl;
    subl->next = tmp;
    return true;
}

short init_quick_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) malloc(sizeof(Other_info));
    if(other_info == NULL)
        return SORT_FAILURE;

    other_info->heap = NULL;
    other_info->current = NULL;
    if(!add_to_heap(other_info, 0, info->array_len - 1)) {
        free(other_info);
        return SORT_FAILURE;
    }

    other_info->current = other_info->heap;
    other_info->pivot = other_info->current->end;
    other_info->last_i = other_info->current->start;
    info->other = (void*) other_info;
    info->cursor = 0;
    return SORT_SUCCESS;
}

short quick_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) info->other;
    Sub_list* current = other_info->current;

    if(info->cursor >= current->end) {
        swap(info, other_info->last_i, other_info->pivot);

        if(other_info->last_i < current->end && !add_to_heap(other_info, other_info->last_i + 1, current->end))
            return SORT_FAILURE;

        if(other_info->last_i > current->start && !add_to_heap(other_info, current->start, other_info->last_i - 1))
            return SORT_FAILURE;

        if(other_info->current->next == NULL)
            return SORT_FINISHED;

        other_info->current = other_info->current->next;
        other_info->pivot = other_info->current->end;
        other_info->last_i = other_info->current->start;
        info->cursor = other_info->current->start;
        return SORT_SUCCESS;
    }

    if(info->array[info->cursor] <= info->array[other_info->pivot]) {
        swap(info, info->cursor, other_info->last_i);
        other_info->last_i++;
    }

    info->cursor++;
    return SORT_SUCCESS;
}

void free_quick_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) info->other;
    free_heap(other_info->heap);
    free(other_info);
}
#include <stdlib.h>
#include <stdbool.h>
#include "sort.h"
#include "merge_bottom_up.h"

typedef struct Other_info {
    int* list1;
    int* list2;
    int lists_start, lists_len, list1_cursor, list2_cursor, list1_len, list2_len;
} Other_info;

int copy_n_elements(Sort_info* info, int* dest, int start, int n) {
    int size = 0;

    for(int i = 0; start + i < info->array_len && i < n; i++) {
        dest[i] = info->array[start + i];
        size++;
    }

    return size;
}

short init_merge_bottom_up_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) malloc(sizeof(Other_info));
    if(other_info == NULL)
        return SORT_FAILURE;

    other_info->list1 = (int*) malloc(sizeof(int) * info->array_len);
    if(other_info->list1 == NULL) {
        free(other_info);
        return SORT_FAILURE;
    }

    other_info->list2 = (int*) malloc(sizeof(int) * info->array_len);
    if(other_info->list2 == NULL) {
        free(other_info->list1);
        free(other_info);
        return SORT_FAILURE;
    }

    other_info->lists_len = 1;
    other_info->lists_start = 0;
    other_info->list1_cursor = 0;
    other_info->list2_cursor = 0;
    other_info->list1_len = copy_n_elements(info, other_info->list1, other_info->lists_start, other_info->lists_len);
    other_info->list2_len = copy_n_elements(info, other_info->list2, other_info->lists_start + other_info->lists_len, other_info->lists_len);

    info->other = (void*) other_info;
    info->cursor = other_info->lists_start;
    return SORT_SUCCESS;
}

short merge_bottom_up_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) info->other;

    if(other_info->lists_len >= info->array_len)
        return SORT_FINISHED;

    bool list1_empty = other_info->list1_cursor >= other_info->list1_len;
    bool list2_empty = other_info->list2_cursor >= other_info->list2_len;
    if(!list1_empty || !list2_empty) {
        int l1 = other_info->list1_cursor, l2 = other_info->list2_cursor;
        if(!list1_empty && (list2_empty || other_info->list1[other_info->list1_cursor] < other_info->list2[other_info->list2_cursor])) {
            info->array[info->cursor] = other_info->list1[other_info->list1_cursor];
            other_info->list1_cursor++;
        } else {
            info->array[info->cursor] = other_info->list2[other_info->list2_cursor];
            other_info->list2_cursor++;
        }
        
        info->cursor++;
    } else {
        other_info->lists_start += other_info->lists_len * 2;

        if(other_info->lists_start + other_info->lists_len > info->array_len) {
            other_info->lists_start = 0;
            other_info->lists_len *= 2;
        }

        info->cursor = other_info->lists_start;
        other_info->list1_cursor = 0;
        other_info->list2_cursor = 0;
        other_info->list1_len = copy_n_elements(info, other_info->list1, other_info->lists_start, other_info->lists_len);
        other_info->list2_len = copy_n_elements(info, other_info->list2, other_info->lists_start + other_info->lists_len, other_info->lists_len);
    }

    return SORT_SUCCESS;
}

void free_merge_bottom_up_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) info->other;
    free(other_info->list1);
    free(other_info->list2);
    free(other_info);
}
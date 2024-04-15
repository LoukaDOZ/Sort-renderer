#include <stdlib.h>
#include <stdbool.h>
#include "sort.h"
#include "merge_top_down.h"

typedef struct Sub_list Sub_list;
struct Sub_list {
    Sub_list* parent;
    Sub_list* sub_list1;
    Sub_list* sub_list2;
    int* merged_list;
    int start, end, cursor;
};

typedef struct Other_info {
    Sub_list* root;
    Sub_list* current;
} Other_info;

Sub_list* create_sub_list(Sub_list* parent, int start, int end) {
    Sub_list* subl = (Sub_list*) malloc(sizeof(Sub_list));
    if(subl == NULL)
        return NULL;

    subl->merged_list = (int*) malloc(sizeof(int) * (end - start + 1));
    if(subl->merged_list == NULL) {
        free(subl);
        return NULL;
    }

    subl->parent = parent;
    subl->sub_list1 = NULL;
    subl->sub_list2 = NULL;
    subl->start = start;
    subl->end = end;
    subl->cursor = 0;
    return subl;
}

void free_sub_list(Sub_list* subl) {
    if(subl == NULL)
        return;

    free(subl->merged_list);
    free_sub_list(subl->sub_list1);
    free_sub_list(subl->sub_list2);
}

short init_merge_top_down_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) malloc(sizeof(Other_info));
    if(other_info == NULL)
        return SORT_FAILURE;

    other_info->root = create_sub_list(NULL, 0, info->array_len - 1);
    if(other_info->root == NULL) {
        free(other_info);
        return SORT_FAILURE;
    }

    other_info->current = other_info->root;
    info->other = (void*) other_info;
    info->cursor = other_info->current->start;
    return SORT_SUCCESS;
}

short merge_top_down_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) info->other;
    Sub_list* current = other_info->current;

    if(current == NULL)
        return SORT_FINISHED;

    if(current->start == current->end) {
        current->merged_list[0] = info->array[current->start];
        other_info->current = current->parent;
        info->cursor = other_info->current->start;
        return SORT_SUCCESS;
    }
    
    if(current->sub_list1 == NULL) {
        current->sub_list1 = create_sub_list(current, current->start, current->start + (current->end - current->start) / 2);
        other_info->current = current->sub_list1;
        info->cursor = other_info->current->start;
        return SORT_SUCCESS;
    }

    if(current->sub_list2 == NULL) {
        current->sub_list2 = create_sub_list(current, current->start + (current->end - current->start) / 2 + 1, current->end);
        other_info->current = current->sub_list2;
        info->cursor = other_info->current->start;
        return SORT_SUCCESS;
    }

    int cursor1 = current->sub_list1->cursor;
    int cursor2 = current->sub_list2->cursor;
    int* merged1 = current->sub_list1->merged_list;
    int* merged2 = current->sub_list2->merged_list;
    bool list1_empty = current->sub_list1->start + cursor1 > current->sub_list1->end;
    bool list2_empty = current->sub_list2->start + cursor2 > current->sub_list2->end;

    if(list1_empty && list2_empty) {
        current->cursor = 0;
        other_info->current = current->parent;
        info->cursor = current->start;
        return SORT_SUCCESS;
    } else if(!list1_empty && (list2_empty || merged1[cursor1] <= merged2[cursor2])) {
        current->merged_list[current->cursor] = merged1[cursor1];
        current->sub_list1->cursor++;
    } else {
        current->merged_list[current->cursor] = merged2[cursor2];
        current->sub_list2->cursor++;
    }

    info->array[info->cursor] = current->merged_list[current->cursor];
    info->cursor = current->start + current->cursor;
    current->cursor++;
    return SORT_SUCCESS;
}

void free_merge_top_down_sort(Sort_info* info) {
    Other_info* other_info = (Other_info*) info->other;
    free_sub_list(other_info->root);
    free(other_info);
}
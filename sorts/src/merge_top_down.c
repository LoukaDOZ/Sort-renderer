#include <stdlib.h>
#include <stdbool.h>
#include "api.h"
#include "sorts.h"

static int* copy_elements(Data* data, int start, int len) {
    int* copy = (int*) malloc(sizeof(int) * len);
    if(copy == NULL)
        return NULL;

    for(int i = 0; i < len; i++)
        copy[i] = data->array[start + i];

    return copy;
}

static bool sort_sub_array(Data* data, int start, int len) {
    if(len <= 1)
        return true;

    int mid = len / 2;
    int start1 = start, len1 = mid, cursor1 = 0;
    int start2 = start + mid, len2 = len - mid, cursor2 = 0;
    if(!sort_sub_array(data, start1, len1) || !sort_sub_array(data, start2, len2))
        return false;

    int* list1 = copy_elements(data, start1, len1);
    if(list1 == NULL)
        return false;

    int* list2 = copy_elements(data, start2, len2);
    if(list1 == NULL) {
        free(list1);
        return false;
    }

    for(int i = 0; i < len && run(data); i++) {
        int array_i = start + i;

        if(cursor1 < len1 && (cursor2 >= len2 || list1[cursor1] < list2[cursor2])) {
            data->array[array_i] = list1[cursor1];
            cursor1++;
        } else {
            data->array[array_i] = list2[cursor2];
            cursor2++;
        }

        data->cursor = array_i;
        tick(data);
    }

    free(list1);
    free(list2);
    return true;
}

short run_merge_top_down_sort(Data* data) {
    if(!sort_sub_array(data, 0, data->array_len))
        return SORT_FAILURE;

    return SORT_SUCCESS;
}
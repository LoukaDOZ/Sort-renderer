#include <stdlib.h>
#include <stdbool.h>
#include "sorts.h"

#define MAX_RUN_SIZE 64
#define MIN_RUN_SIZE 4
#define MIN_RUN_COUNT 8

static int* copy_elements(Data* data, int start, int len) {
    int* copy = (int*) malloc(sizeof(int) * len);
    if(copy == NULL)
        return NULL;

    for(int i = 0; i < len; i++)
        copy[i] = data->array[start + i];

    return copy;
}

static void insertion_sort(Data* data, int start, int len) {
    for(int i = 1; i < len && run(data); i++) {
        for(int j = i; j > 0; j--) {
            if(data->array[start + j - 1] > data->array[start + j])
                swap(data, start + j - 1, start + j);
            else
                break;

            data->cursor = start + j - 1;
            tick(data);
        }
    }
}

static bool sort_sub_array(Data* data, int start, int len, int run_size) {
    if(len <= 1)
        return true;

    if(len <= run_size) {
        insertion_sort(data, start, len);
        return true;
    }

    int mid = len / 2;
    int start1 = start, len1 = mid, cursor1 = 0;
    int start2 = start + mid, len2 = len - mid, cursor2 = 0;
    if(!sort_sub_array(data, start1, len1, run_size) || !sort_sub_array(data, start2, len2, run_size))
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

short run_tim_sort(Data* data) {
    int run_size = MAX_RUN_SIZE;
    for(; run_size > MIN_RUN_SIZE && data->array_len / run_size < MIN_RUN_COUNT; run_size /= 2);

    if(!sort_sub_array(data, 0, data->array_len, run_size))
        return SORT_FAILURE;

    return SORT_SUCCESS;
}
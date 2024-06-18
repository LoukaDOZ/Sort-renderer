#include <stdlib.h>
#include "sorts.h"

static int* copy_elements(Data* data, int start, int len) {
    int* copy = (int*) malloc(sizeof(int) * len);
    if(copy == NULL)
        return NULL;

    for(int i = 0; i < len; i++)
        copy[i] = data->array[start + i];

    return copy;
}

short run_merge_bottom_up_sort(Data* data) {
    for(int size = 1; size < data->array_len && run(data); size *= 2) {
        for(int start = 0; start + size < data->array_len && run(data); start += size * 2) {
            int start1 = start, len1 = size, cursor1 = 0;
            int start2 = start + size, len2 = start2 + size <= data->array_len ? size : data->array_len - start2, cursor2 = 0;

            int* list1 = copy_elements(data, start1, len1);
            if(list1 == NULL)
                return false;

            int* list2 = copy_elements(data, start2, len2);
            if(list1 == NULL) {
                free(list1);
                return false;
            }

            for(int i = 0; i < len1 + len2 && run(data); i++) {
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
        }
    }

    return SORT_SUCCESS;
}
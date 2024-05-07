#include <stdlib.h>
#include "api.h"
#include "sorts.h"

static void sort_sub_array(Data* data, int start, int end) {
    if(end <= start)
        return;

    int pivot = data->array[end];
    int last_changed = start;

    for(int i = start; i < end; i++) {
        if(!run(data))
            return;

        if(data->array[i] <= pivot) {
            swap(data, i, last_changed);
            last_changed++;
        }

        data->cursor = i;
        tick(data);
    }

    swap(data, last_changed, end);
    sort_sub_array(data, start, last_changed - 1);
    sort_sub_array(data, last_changed + 1, end);
}

short run_quick_sort(Data* data) {
    sort_sub_array(data, 0, data->array_len - 1);
    tick(data);
    return SORT_SUCCESS;
}
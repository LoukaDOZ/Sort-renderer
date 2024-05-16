#include <stdlib.h>
#include <math.h>
#include "api.h"
#include "sorts.h"

static void slow_sort(Data* data, int start, int end) {
    if(start >= end || !run(data))
        return;

    int mid = (start + end) / 2;

    slow_sort(data, start, mid);
    slow_sort(data, mid + 1, end);

    if(data->array[end] < data->array[mid])
        swap(data, end, mid);
    
    data->cursor = mid;
    tick(data);

    slow_sort(data, start, end - 1);
}

short run_slow_sort(Data* data) {
    slow_sort(data, 0, data->array_len - 1);
    return SORT_SUCCESS;
}
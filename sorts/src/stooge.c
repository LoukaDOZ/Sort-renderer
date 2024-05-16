#include <stdlib.h>
#include <math.h>
#include "api.h"
#include "sorts.h"

static void stooge_sort(Data* data, int start, int len) {
    if(start >= start + len - 1 || !run(data))
        return;

    if(data->array[start] > data->array[start + len - 1])
        swap(data, start, start + len - 1);

    data->cursor = start;
    tick(data);

    if(len <= 2)
        return;

    int two_thirds = (int) ceil(((double) len) * (2.0 / 3.0));

    stooge_sort(data, start, two_thirds);
    stooge_sort(data, start + len - two_thirds, two_thirds);
    stooge_sort(data, start, two_thirds);
}

short run_stooge_sort(Data* data) {
    stooge_sort(data, 0, data->array_len);
    return SORT_SUCCESS;
}
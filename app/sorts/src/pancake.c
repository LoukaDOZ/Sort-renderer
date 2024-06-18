#include <stdlib.h>
#include "sorts.h"

static int find_max_index(Data* data, int limit) {
    int max_i = 0;

    for(int i = 1; i < limit && run(data); i++) {
        if(data->array[max_i] < data->array[i])
            max_i = i;

        data->cursor = i;
        tick(data);
    }

    return max_i;
}

static void flip(Data* data, int limit) {
    for(int i = 0; i < limit && run(data); i++, limit--) {
        swap(data, i, limit);
        data->cursor = i;
        tick(data);
    }
}

short run_pancake_sort(Data* data) {
    for(int i = data->array_len - 1; i > 0 && run(data); i--) {
        int max_i = find_max_index(data, i + 1);

        if(max_i < i) {
            flip(data, max_i);
            flip(data, i);
        }

        data->cursor = i;
        tick(data);
    }
    
    return SORT_SUCCESS;
}
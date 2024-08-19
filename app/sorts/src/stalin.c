#include <stdlib.h>
#include <string.h>
#include "sorts.h"

short run_stalin_sort(Data* data) {
    for(int i = 1; i < data->array_len && run(data);) {
        if(data->array[i - 1] > data->array[i]) {
            memmove(data->array + i, data->array + i + 1, sizeof(int) * (data->array_len - i - 1));
            data->array_len--;
        } else
             i++;

        data->cursor = i;
        tick(data);
    }

    return SORT_SUCCESS;
}
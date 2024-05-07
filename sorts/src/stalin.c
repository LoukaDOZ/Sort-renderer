#include <stdlib.h>
#include "api.h"
#include "sorts.h"

short run_stalin_sort(Data* data) {
    for(int i = 1; i < data->array_len && run(data);) {
        if(data->array[i - 1] > data->array[i]) {
            for(int j = i + 1; j < data->array_len; j++)
                data->array[j - 1] = data->array[j];

            data->array_len--;
        } else
             i++;

        data->cursor = i;
        tick(data);
    }

    return SORT_SUCCESS;
}
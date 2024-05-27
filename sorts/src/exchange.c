#include <stdlib.h>
#include "api.h"
#include "sorts.h"

short run_exchange_sort(Data* data) {
    for(int i = 0; i < data->array_len && run(data); i++) {
        for(int j = i + 1; j < data->array_len && run(data); j++) {
            if(data->array[j] < data->array[i])
                swap(data, i, j);

            data->cursor = j;
            tick(data);
        }
    }

    return SORT_SUCCESS;
}
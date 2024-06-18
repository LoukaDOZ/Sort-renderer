#include <stdlib.h>
#include "sorts.h"

short run_double_selection_sort(Data* data) {
    for(int i = 0; i < data->array_len - i && run(data); i++) {
        int smallest = i;
        int biggest = data->array_len - 1 - i;

        if(data->array[smallest] > data->array[biggest]) {
            swap(data, smallest, biggest);
            data->cursor = smallest;
            tick(data);
        }

        for(int j = smallest; j < data->array_len - i && run(data); j++) {
            if(data->array[j] < data->array[smallest])
                smallest = j;
            else if(data->array[j] > data->array[biggest])
                biggest = j;

            data->cursor = j;
            tick(data);
        }

        swap(data, i, smallest);
        swap(data, data->array_len - 1 - i, biggest);

        data->cursor = i;
        tick(data);

        data->cursor = data->array_len - 1 - i;
        tick(data);
    }

    return SORT_SUCCESS;
}
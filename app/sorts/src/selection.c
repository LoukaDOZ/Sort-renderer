#include <stdlib.h>
#include "sorts.h"

short run_selection_sort(Data* data) {
    for(int i = 0; i < data->array_len && run(data); i++) {
        int smallest = i;

        for(int j = smallest + 1; j < data->array_len && run(data); j++) {
            if(data->array[j] < data->array[smallest])
                smallest = j;

            data->cursor = j;
            tick(data);
        }

        swap(data, i, smallest);
        data->cursor = i;
        tick(data);
    }

    return SORT_SUCCESS;
}
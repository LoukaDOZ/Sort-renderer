#include <stdlib.h>
#include "sorts.h"

short run_bubble_sort(Data* data) {
    bool exchanged = true;

    for(int i = data->array_len - 1; exchanged && i > 0 && run(data); i--) {
        exchanged = false;

        for(int j = 0; j < i && run(data); j++) {
            if(data->array[j] > data->array[j + 1]) {
                swap(data, j, j + 1);
                exchanged = true;
            }

            data->cursor = j + 1;
            tick(data);
        }
    }

    return SORT_SUCCESS;
}
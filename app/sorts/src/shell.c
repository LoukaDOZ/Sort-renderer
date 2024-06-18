#include <stdlib.h>
#include "sorts.h"

short run_shell_sort(Data* data) {
    for(int gap = (int) (data->array_len / 2); gap > 0 && run(data); gap = (int) (gap / 2)) {
        for(int i = gap; i < data->array_len; i++) {
            int tmp = data->array[i];
            int j = i;

            for(; j >= gap && data->array[j - gap] > tmp && run(data); j -= gap) {
                data->array[j] = data->array[j - gap];
                data->cursor = j - gap;
                tick(data);
            }

            data->array[j] = tmp;
        }
    }

    return SORT_SUCCESS;
}
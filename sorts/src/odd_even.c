#include <stdlib.h>
#include <stdbool.h>
#include "api.h"
#include "sorts.h"

short run_odd_even_sort(Data* data) {
    bool is_sorted = false;

    while(!is_sorted && run(data)) {
        is_sorted = true;

        for(int i = 1; i < data->array_len - 1 && run(data); i += 2) {
            if(data->array[i] > data->array[i + 1]) {
                swap(data, i, i + 1);
                is_sorted = false;
            }

            data->cursor = i;
            tick(data);
        }

        for(int i = 0; i < data->array_len - 1 && run(data); i += 2) {
            if(data->array[i] > data->array[i + 1]) {
                swap(data, i, i + 1);
                is_sorted = false;
            }

            data->cursor = i;
            tick(data);
        }
    }

    return SORT_SUCCESS;
}
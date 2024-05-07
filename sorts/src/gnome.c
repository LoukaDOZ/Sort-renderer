#include <stdlib.h>
#include "api.h"
#include "sorts.h"

short run_gnome_sort(Data* data) {
    for(int i = 0; i < data->array_len && run(data);) {
        if(i == 0 || data->array[i] >= data->array[i - 1]) {
            i++;
        } else {
            swap(data, i, i - 1);
            i--;
        }

        data->cursor = i;
        tick(data);
    }

    return SORT_SUCCESS;
}
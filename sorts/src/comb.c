#include <stdlib.h>
#include "api.h"
#include "sorts.h"

short run_comb_sort(Data* data) {
    bool exchanged = true;
    int gap = data->array_len;

    while(exchanged && run(data)) {
        gap = (int) (((double) gap) / 1.3);
        exchanged = false;

        if(gap < 1)
            gap = 1;

        for(int i = 0; i < data->array_len - gap && run(data); i++) {
            if(data->array[i] > data->array[i + gap]) {
                swap(data, i, i + gap);
                exchanged = true;
            }

            data->cursor = i;
            tick(data);
        }
    }

    return SORT_SUCCESS;
}
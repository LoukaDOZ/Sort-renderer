#include <stdlib.h>
#include <stdbool.h>
#include "api.h"
#include "sorts.h"

short run_shaker_sort(Data* data) {
    int start = 0, end = data->array_len - 1;

    while(run(data)) {
        bool exchanged = false;
        int i = start;

        for(; i < end && run(data); i++) {
            if(data->array[i] > data->array[i + 1]) {
                swap(data, i, i + 1);
                exchanged = true;
            }

            data->cursor = i;
            tick(data);
        }

        for(; i > start && run(data); i--) {
            if(data->array[i - 1] > data->array[i]) {
                swap(data, i - 1, i);
                exchanged = true;
            }

            data->cursor = i;
            tick(data);
        }

        if(!exchanged)
            break;
        
        start++;
        end--;
    }

    return SORT_SUCCESS;
}
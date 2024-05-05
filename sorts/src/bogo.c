#include <stdlib.h>
#include <stdbool.h>
#include "api.h"
#include "bogo.h"

short run_bogo_sort(Data* data) {
    while(run(data)) {
        for(int i = 0; i < data->array_len && run(data); i++) {
            int rand_i = rand() % data->array_len;
            swap(data, i, rand_i);

            data->cursor = i;
            tick(data);
        }

        bool sorted = true;
        for(int i = 1; i < data->array_len && run(data); i++) {
            if(data->array[i - 1] > data->array[i]) {
                sorted = false;
                break;
            }
        }

        if(sorted)
            break;
    }
    
    return SORT_SUCCESS;
}
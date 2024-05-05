#include <stdlib.h>
#include "api.h"
#include "insertion.h"

short run_insertion_sort(Data* data) {
    for(int i = 1; i < data->array_len && run(data); i++) {        
        for(int j = i; j > 0 && run(data); j--) {
            if(data->array[j - 1] > data->array[j])
                swap(data, j - 1, j);
            else
                break;

            data->cursor = j - 1;
            tick(data);
        }
    }

    return SORT_SUCCESS;
}
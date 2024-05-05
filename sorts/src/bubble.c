#include <stdlib.h>
#include "api.h"
#include "bubble.h"

short run_bubble_sort(Data* data) {
    for(int i = data->array_len - 1; i > 0 && data->run; i--) {        
        for(int j = 0; j < i && data->run; j++) {
            if(data->array[j] > data->array[j + 1])
                swap(data, j, j + 1);

            data->cursor = j + 1;
            tick(data);
        }
    }

    return SORT_SUCCESS;
}
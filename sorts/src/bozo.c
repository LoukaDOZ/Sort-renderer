#include <stdlib.h>
#include <stdbool.h>
#include "api.h"
#include "sorts.h"

short run_bozo_sort(Data* data) {
    while(run(data)) {
        int a = rand() % data->array_len;
        int b = rand() % data->array_len;
        swap(data, a, b);

        data->cursor = a;
        tick(data);

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
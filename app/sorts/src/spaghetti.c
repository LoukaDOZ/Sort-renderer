#include <stdlib.h>
#include "sorts.h"

short run_spaghetti_sort(Data* data) {
    int min = data->array[0], max = data->array[0];

    for(int i = 0; i < data->array_len && run(data); i++) {
        if(data->array[i] < min)
            min = data->array[i];
        else if(data->array[i] > max)
            max = data->array[i];
    }

    int end = data->array_len - 1;
    for(int hand = max; hand >= min && run(data); hand--) {
        int found = -1;

        for(int i = 0; i < data->array_len && run(data); i++) {
            if(data->array[i] == hand) {
                found = i;
                break;
            }

            data->cursor = i;
            tick(data);
        }

        if(found >= 0) {
            swap(data, end, found);
            data->cursor = end;
            tick(data);
            end--;
        } else
            hand--;
    }

    return SORT_SUCCESS;
}
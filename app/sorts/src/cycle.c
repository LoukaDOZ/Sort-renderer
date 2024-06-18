#include <stdlib.h>
#include "sorts.h"

short run_cycle_sort(Data* data) {
    for(int cycle_start = 0; cycle_start < data->array_len - 1 && run(data); cycle_start++) {
        int item = data->array[cycle_start], pos = cycle_start;

        for(int i = cycle_start + 1; i < data->array_len && run(data); i++) {
            if(data->array[i] < item) {
                pos++;
                data->cursor = pos;
                tick(data);
            }
        }

        if(pos == cycle_start)
            continue;

        while(data->array[pos] == item && run(data)) {
            pos++;
            data->cursor = pos;
            tick(data);
        }

        int tmp = item;
        item = data->array[pos];
        data->array[pos] = tmp;

        while(pos != cycle_start && run(data)) {
            pos = cycle_start;

            for(int i = cycle_start + 1; i < data->array_len && run(data); i++) {
                if(data->array[i] < item) {
                    pos++;
                    data->cursor = pos;
                    tick(data);
                }
            }

            while(data->array[pos] == item && run(data)) {
                pos++;
                data->cursor = pos;
                tick(data);
            }

            int tmp = item;
            item = data->array[pos];
            data->array[pos] = tmp;
        }
    }

    return SORT_SUCCESS;
}
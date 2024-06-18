#include <stdlib.h>
#include "sorts.h"

short run_strand_sort(Data* data) {
    int output_array[data->array_len];
    int output_len = 0;

    while(output_len < data->array_len && run(data)) {
        int tmp_len = 1;

        for(int i = output_len + 1; i < data->array_len && run(data); i++) {
            if(data->array[i] >= data->array[output_len + tmp_len - 1]) {
                swap(data, output_len + tmp_len, i);
                tmp_len++;
            }

            data->cursor = i;
            tick(data);
        }

        int cursor1 = 0, cursor2 = 0;
        for(int i = 0; i < output_len + tmp_len; i++) {
            if(cursor2 >= tmp_len || (cursor1 < output_len && data->array[cursor1] < data->array[output_len + cursor2])) {
                output_array[i] = data->array[cursor1];
                cursor1++;
            } else {
                output_array[i] = data->array[output_len + cursor2];
                cursor2++;
            }
        }

        for(int i = 0; i < output_len + tmp_len; i++) {
            data->array[i] = output_array[i];
            data->cursor = i;
            tick(data);
        }

        output_len += tmp_len;
    }

    return SORT_SUCCESS;
}
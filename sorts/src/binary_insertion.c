#include <stdlib.h>
#include "api.h"
#include "sorts.h"

static int binary_search(Data* data, int start, int end, int value) {
    if(end <= start || !run(data))
        return data->array[start] > value ? start : start + 1;

    int mid = start + (end - start) / 2;
    data->cursor = mid;
    tick(data);

    if(data->array[mid] == value)
        return mid + 1;
    if(data->array[mid] > value)
        return binary_search(data, start, mid - 1, value);
    
    return binary_search(data, mid + 1, end, value);
}

short run_binary_insertion_sort(Data* data) {
    for(int i = 1; i < data->array_len && run(data); i++) {
        int pos = binary_search(data, 0, i, data->array[i]);
        
        for(int j = i; j > pos && run(data); j--) {
            swap(data, j - 1, j);
            data->cursor = j - 1;
            tick(data);
        }
    }
    return SORT_SUCCESS;
}
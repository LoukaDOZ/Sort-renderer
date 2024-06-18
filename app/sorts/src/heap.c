#include <stdlib.h>
#include "sorts.h"

static void heapify(Data* data, int i, int array_len) {
    if(!run(data))
        return;

    int largest = i;
    int lchild = 2 * i + 1;
    int rchild = 2 * i + 2;
    
    if(lchild < array_len && data->array[largest] < data->array[lchild])
        largest = lchild;
    
    if(rchild < array_len && data->array[largest] < data->array[rchild])
        largest = rchild;

    data->cursor = i;
    tick(data);

    if(largest != i) {
        swap(data, i, largest);
        heapify(data, largest, array_len);
    }
}

static void build_max_heap(Data* data) {
    for(int last_leaf = data->array_len / 2 - 1; last_leaf >= 0 && run(data); last_leaf--)
        heapify(data, last_leaf, data->array_len);
}

short run_heap_sort(Data* data) {
    build_max_heap(data);

    for(int end = data->array_len - 1; end > 0 && run(data); end--) {
        swap(data, 0, end);
        heapify(data, 0, end);

        data->cursor = end;
        tick(data);
    }

    return SORT_SUCCESS;
}
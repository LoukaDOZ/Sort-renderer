#include <stdlib.h>
#include <stdint.h>
#include "sorts.h"

const int leonardo_len = 46;
const int leonardo[] = {
    1, 1, 3, 5, 9, 15, 25, 41, 67, 109, 177, 287, 465, 753, 
    1219, 1973, 3193, 5167, 8361, 13529, 21891, 35421, 57313, 92735,
    150049, 242785, 392835, 635621, 1028457, 1664079, 2692537, 
    4356617, 7049155, 11405773, 18454929, 29860703, 48315633, 78176337,
    126491971, 204668309, 331160281, 535828591, 866988873, 1402817465,
    2269806339, 3672623805
};
 
static void heapify(Data* data, int start, int root_index, int size) {
    if(size < 2 || !run(data))
        return;

    int largest_size = size;
    int largest_index = root_index;
    int lchild_index = start + leonardo[size - 1] - 1;
    int rchild_index = root_index - 1;
    
    if(lchild_index >= start && data->array[largest_index] < data->array[lchild_index]) {
        largest_size = size - 1;
        largest_index = lchild_index;
    }
    
    if(rchild_index >= start && data->array[largest_index] < data->array[rchild_index]) {
        largest_size = size - 2;
        largest_index = rchild_index;
        start = lchild_index + 1;
    }

    data->cursor = root_index;
    tick(data);

    if(largest_size != size) {
        swap(data, root_index, largest_index);
        heapify(data, start, largest_index, largest_size);
    }
}

static void reheapify(Data* data, uint64_t sizes, int last_heap_size, int array_len) {
    if(array_len <= 1)
        return;

    uint64_t tmp_sizes = sizes;
    int new_size = last_heap_size;
    int new_root_index = array_len - 1;
    int compare_root_index = new_root_index - leonardo[new_size];

    while(compare_root_index >= 0 && run(data)) {
        int new_lchild_index = new_size >= 2 ? new_root_index - leonardo[new_size - 2] - 1 : -1;
        int new_rchild_index = new_size >= 2 ? new_root_index - 1 : -1;

        if(data->array[new_root_index] < data->array[compare_root_index]
            && (new_lchild_index < 0 || data->array[new_lchild_index] < data->array[compare_root_index])
            && (new_rchild_index < 0 || data->array[new_rchild_index] < data->array[compare_root_index])
        ) {
            swap(data, new_root_index, compare_root_index);
        } else
            break;

        do {
            tmp_sizes >>= 1;
            new_size++;
        } while((tmp_sizes & 1) == 0 && run(data));

        new_root_index = compare_root_index;
        compare_root_index = new_root_index - leonardo[new_size];

        data->cursor = new_root_index;
        tick(data);
    }

    if(run(data))
        heapify(data, new_root_index - leonardo[new_size] + 1, new_root_index, new_size);
}

static void queue(Data* data, uint64_t* sizes, int* last_heap_size, int i) {
    if((*sizes & 3) == 3) {
        *sizes >>= 2;
        *sizes += 1;
        *last_heap_size += 2;
    } else if(*last_heap_size == 1) {
        *sizes <<= 1;
        *sizes += 1;
        *last_heap_size = 0;
    } else {
        *sizes <<= *last_heap_size - 1;
        *sizes += 1;
        *last_heap_size = 1;
    }

    reheapify(data, *sizes, *last_heap_size, i + 1);
}

static void dequeue(Data* data, uint64_t* sizes, int* last_heap_size, int i) {
    int last_size = *last_heap_size;

    if(last_size >= 2) {
        *sizes -= 1;
        
        *sizes <<= 1;
        *sizes += 1;
        *last_heap_size -= 1;
        reheapify(data, *sizes, *last_heap_size, i - leonardo[last_size - 2]);

        *sizes <<= 1;
        *sizes += 1;
        *last_heap_size -= 1;
        reheapify(data, *sizes, *last_heap_size, i);
    } else if(last_size == 0) {
        *sizes >>= 1;
        *last_heap_size = 1;
    } else {
        do {
            *sizes >>= 1;
            (*last_heap_size)++;
        } while((*sizes & 1) == 0 && i - leonardo[last_size] >= 0);
    }
}

short run_smooth_sort(Data* data) {
    uint64_t sizes = 0;
    int last_heap_size = 0;
    
    for(int i = 0; i < data->array_len && run(data); i++)
        queue(data, &sizes, &last_heap_size, i);

    for(int i = data->array_len - 1; i >= 0 && run(data); i--)
        dequeue(data, &sizes, &last_heap_size, i);
    
    return SORT_SUCCESS;
}
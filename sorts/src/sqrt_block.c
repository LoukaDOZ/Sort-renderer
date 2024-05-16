#include <stdlib.h>
#include <math.h>
#include "api.h"
#include "sorts.h"

#define A_BLOCK 0
#define B_BLOCK 1

static int bvalue(Data* data, int start, int size, short type) {
    return type == A_BLOCK ? data->array[start] : data->array[start + size - 1];
}

static void roll_one(Data* data, int source_start, int dest_start, int size, short* source_type, short* dest_type){
    for(int i = 0; i < size && run(data); i++) {
        swap(data, source_start + i, dest_start + i);
        data->cursor = source_start + i;
        tick(data);
    }

    short tmp = *source_type;
    *source_type = *dest_type;
    *dest_type = tmp;
}

static void roll_and_drop(Data* data, int start, int nb_blocks, int block_size, short* types) {
    int nb_A_placed = 0, nb_B_placed = 0;

    while(nb_A_placed < nb_blocks && nb_B_placed < nb_blocks && run(data)) {
        int start_A = start + (nb_A_placed + nb_B_placed) * block_size;
        int start_B = start_A + (nb_blocks - nb_A_placed) * block_size;

        if(bvalue(data, start_A, block_size, A_BLOCK) <= bvalue(data, start_B, block_size, B_BLOCK)) {
            // Drop
            nb_A_placed++;
        } else {
            // Roll
            for(int i = nb_blocks - nb_A_placed - 1; i >= 0; i--)
                roll_one(data, start_A + i * block_size, start_A + (i + 1) * block_size, block_size, &(types[nb_A_placed + nb_B_placed + i]), &(types[nb_A_placed + nb_B_placed + i + 1]));

            nb_B_placed++;
        }
        
        data->cursor = start_A;
        tick(data);
    }
}

static void local_merge(Data* data, int mega_block_start, int mega_block_size, int merge_start, int merge_size) {
    for(int i = merge_start + merge_size - 1; i >= merge_start && run(data); i--) {
        for(int j = i; j < mega_block_start + mega_block_size - 1 && run(data); j++) {
            if(data->array[j] > data->array[j + 1])
                swap(data, j, j + 1);
            else
                break;

            data->cursor = j;
            tick(data);
        }
    }
}

static void merge(Data* data, int start, int size, int blocks_start, int nb_blocks, int block_size, short* types) {
    int end = start + size;
    int mega_block_start = blocks_start + nb_blocks * 2 * block_size;
    int mega_block_size = end - mega_block_start;
    int current_block = nb_blocks * 2 - 1;
    short mega_block_type = B_BLOCK;

    data->cursor = mega_block_start;
    tick(data);

    // If no right excess block
    if(mega_block_start == end) {
        mega_block_start = end - block_size;
        mega_block_size = end - mega_block_start;
        //mega_block_type = types[current_block];
        current_block--;

        data->cursor = mega_block_start;
        tick(data);
    }

    for(; current_block >= 0 && run(data); current_block--) {
        if(mega_block_type != types[current_block])
            local_merge(data, mega_block_start, mega_block_size, blocks_start + current_block * block_size, block_size);

        mega_block_start -= block_size;
        mega_block_size += block_size;
        data->cursor = mega_block_start;
        tick(data);
    }

    // If left excess block
    if(start < mega_block_start && mega_block_type != A_BLOCK && run(data)) {
        local_merge(data, mega_block_start, mega_block_size, start, mega_block_start - start);
        data->cursor = start;
        tick(data);
    }
}

static void sort_sub_array(Data* data, int start, int len) {
    if(len <= 1 || !run(data))
        return;

    int mid = len / 2;
    int start1 = start, len1 = mid;
    int start2 = start + mid, len2 = len - mid;
    
    sort_sub_array(data, start1, len1);
    sort_sub_array(data, start2, len2);

    if(!run(data))
        return;

    int block_size = (int) sqrt(mid); // Also is nb A blocks and nb B blocks
    int blocks_start = block_size * block_size < mid ? start + mid - (block_size * block_size) : start;
    short blocks_type[block_size * 2];

    for(int i = 0; i < block_size * 2; i++)
        blocks_type[i] = i < block_size ? A_BLOCK : B_BLOCK;

    roll_and_drop(data, blocks_start, block_size, block_size, blocks_type);
    merge(data, start, len, blocks_start, block_size, block_size, blocks_type);
}

short run_sqrt_block_sort(Data* data) {
    sort_sub_array(data, 0, data->array_len);
    return SORT_SUCCESS;
}
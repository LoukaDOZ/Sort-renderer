#include <stdlib.h>
#include "sort.h"

Sort_info* create_sort_info(int array_size) {
    Sort_info* info = (Sort_info*) malloc(sizeof(Sort_info));
    if(info == NULL)
        return NULL;
    
    info->array = (int*) malloc(sizeof(int) * array_size);
    if(info->array == NULL) {
        free(info);
        return NULL;
    }

    info->array_len = array_size;
    info->save_array_len = array_size;
    reset_sort_info(info);
    return info;
}

void reset_sort_info(Sort_info* info) {
    for(int i = 0; i < info->save_array_len; i++)
        info->array[i] = i + 1;

    info->array_len = info->save_array_len;
    info->cursor = 0;
}

void free_sort_info(Sort_info* info) {
    free(info->array);
    free(info);
}
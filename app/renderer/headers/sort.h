#ifndef SORT_H
#define SORT_H

typedef struct Sort_info {
    int* array;
    int cursor, array_len, save_array_len;
} Sort_info;

Sort_info* create_sort_info(int array_size);
void reset_sort_info(Sort_info* info);
void free_sort_info(Sort_info* info);

#endif
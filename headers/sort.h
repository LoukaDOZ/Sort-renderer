#ifndef SORT_H
#define SORT_H

extern const short SORT_SUCCESS;
extern const short SORT_FAILURE;
extern const short SORT_FINISHED;

typedef struct Sort_info {
    int cursor, array_len;
    int* array;
    void* other;
} Sort_info;

typedef struct Sort_function {
    char* name;
    char* complexity;
    short (*init)(Sort_info* info);
    short (*sort)(Sort_info* info);
    void (*free)(Sort_info* info);
} Sort_function;

extern const int SORT_FUNCTIONS_LEN;
extern const Sort_function SORT_FUNCTIONS[];

Sort_info* init_sort_info(int array_size);
void free_sort_info(Sort_info* info);

#endif
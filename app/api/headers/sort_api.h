#ifndef SORT_API_H
#define SORT_API_H

#include <stdbool.h>
#include "common_api.h"

extern const short SORT_SUCCESS;
extern const short SORT_FAILURE;

typedef struct Data {
    int* array;
    int cursor, array_len;
    void* _private; // Do not touch
} Data;

bool run(Data* data);
void tick(Data* data);
void swap(Data* data, int a, int b);

#endif
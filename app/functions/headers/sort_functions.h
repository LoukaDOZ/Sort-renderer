#ifndef SORT_FUNCTIONS_H
#define SORT_FUNCTIONS_H

#include "sort_api.h"

typedef short (*Sort_function)(Data* data);

typedef struct Sort_Algorithm {
    char* name;
    char* complexity;
    Sort_function function;
} Sort_Algorithm;

extern const int SORT_ALGORITHMS_LEN;
extern const Sort_Algorithm SORT_ALGORITHMS[];

#endif
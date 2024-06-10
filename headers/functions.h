#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "api.h" // Fix unknown Data

typedef short (*Sort_function)(Data* data);

typedef struct Sort_Algorithm {
    char* name;
    char* complexity;
    Sort_function function;
} Sort_Algorithm;

extern const int SORT_ALGORITHMS_LEN;
extern const Sort_Algorithm SORT_ALGORITHMS[];

#endif
#ifndef API_H
#define API_H

#include <stdbool.h> // Fix unknown bool

extern const short SORT_SUCCESS;
extern const short SORT_FAILURE;

typedef struct Data {
    int cursor, array_len;
    int* array;
    bool run;
    void* _private; // Do not touch
} Data;

void tick(Data* data);

void swap(Data* data, int a, int b);

void debug(char* message, ...);
void debug_warn(char* message, ...);
void debug_err(char* message, ...);

#endif
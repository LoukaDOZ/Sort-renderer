#ifndef API_H
#define API_H

#include <stdbool.h> // Fix unknown bool

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
void debug(char* message, ...);
void debug_warn(char* message, ...);
void debug_err(char* message, ...);

#endif
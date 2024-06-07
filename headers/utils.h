#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h> // Fix unknown bool

extern const long SEC_US;
extern const long SEC_MS;
extern const long MS_US;
extern const long MIN_SEC;

extern const unsigned long MIN_SIMULATION_DELAY;
extern const unsigned long MAX_SIMULATION_DELAY;

bool inverse_bool(bool b);

int max(int a, int b);

long ms_time(void);
long us_time(void);

void decompose_ms(long ms_time, int* min, int* sec, int* ms);
void decompose_us(long us_time, int* min, int* sec, int* ms, int* us);

#endif
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include "utils.h"

const long SEC_US = 1000000;
const long SEC_MS = 1000;
const long MS_US = 1000;
const long MIN_SEC = 60;

const unsigned long MIN_SIMULATION_DELAY = 0;
const unsigned long MAX_SIMULATION_DELAY = SEC_US;

bool inverse_bool(bool b) {
    return (b + 1) % 2;
}

long ms_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * SEC_MS + tv.tv_usec / SEC_MS;
}

long us_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * SEC_US + tv.tv_usec;
}

void decompose_ms(long ms_time, int* min, int* sec, int* ms) {
    *ms = ms_time % SEC_MS;
    *sec = (ms_time / SEC_MS) % MIN_SEC;
    *min = ms_time / (SEC_MS * MIN_SEC);
}

void decompose_us(long us_time, int* min, int* sec, int* ms, int* us) {
    *us = us_time % MS_US;
    *ms = (us_time / MS_US) % SEC_MS;
    *sec = (us_time / SEC_US) % MIN_SEC;
    *min = us_time / (SEC_US * MIN_SEC);
}
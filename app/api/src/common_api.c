#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "common_api.h"

#define DEBUG_LOG_PREFIX "\033[0;34mLOG\033[0m"
#define DEBUG_WARNING_PREFIX "\033[0;33mWARNING\033[0m"
#define DEBUG_ERROR_PREFIX "\033[0;31mERROR\033[0m"

int max(int a, int b) {
    return a < b ? b : a;
}

int min(int a, int b) {
    return a > b ? b : a;
}

int between(int inf, int sup, int v) {
    return min(sup, max(inf, v));
}

float maxf(float a, float b) {
    return a < b ? b : a;
}

float minf(float a, float b) {
    return a > b ? b : a;
}

float betweenf(float inf, float sup, float v) {
    return minf(sup, maxf(inf, v));
}

void print_debug(char* message, char* prefix, va_list args) {
    printf("[%s] ", prefix);
    vprintf(message, args);
    printf("\n");
}

void debug(char* message, ...) {
    va_list args;
    va_start(args, message);
    print_debug(message, DEBUG_LOG_PREFIX, args);
    va_end(args);
}

void debug_warn(char* message, ...) {
    va_list args;
    va_start(args, message);
    print_debug(message, DEBUG_WARNING_PREFIX, args);
    va_end(args);
}

void debug_err(char* message, ...) {
    va_list args;
    va_start(args, message);
    print_debug(message, DEBUG_ERROR_PREFIX, args);
    va_end(args);
}
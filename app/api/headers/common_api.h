#ifndef COMMON_API_H
#define COMMON_API_H

int max(int a, int b);
int min(int a, int b);
int between(int inf, int sup, int v);

float maxf(float a, float b);
float minf(float a, float b);
float betweenf(float inf, float sup, float v);

void debug(char* message, ...);
void debug_warn(char* message, ...);
void debug_err(char* message, ...);

#endif
#ifndef DRAW_FUNCTIONS_H
#define DRAW_FUNCTIONS_H

#include "draw_api.h"

extern const short COLOR_MODE_ALL;
extern const short COLOR_MODE_DEFAULT;
extern const short COLOR_MODE_COLORIZED;

typedef bool (*Draw_function)(Draw_data* data);

typedef struct Draw_function_info {
    Draw_function draw;
    short color_mode;
} Draw_function_info;

extern const int DRAW_FUNCTIONS_LEN;
extern const Draw_function_info DRAW_FUNCTIONS[];

#endif
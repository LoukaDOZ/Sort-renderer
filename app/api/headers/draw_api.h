#ifndef DRAW_API_H
#define DRAW_API_H

#include <stdbool.h>
#include "common_api.h"

typedef struct Draw_data {
    int* array;
    int array_len, current_array_len, cursor, window_w, window_h;
    bool colorized;
    void* _private; // Do not touch
} Draw_data;

typedef struct Color {
    int r, g, b, a;
} Color;

extern const Color NO_COLOR;
extern const Color CURSOR_COLOR;
extern const Color COLORIZED_CURSOR_COLOR;

Color get_color(float ratio);
Color get_color_distinct(float ratio);
Color get_color_greyscale(float ratio);
Color get_color_greyscale_distinct(float ratio);

bool rectangle(Draw_data* data, int x, int y, int w, int h, Color color);
bool triangle(Draw_data* data, int x1, int y1, int x2, int y2, int x3, int y3, Color color);

#endif
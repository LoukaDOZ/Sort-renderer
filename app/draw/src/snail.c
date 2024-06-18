#include <stdlib.h>
#include <math.h>
#include "draw.h"

#define CORRECTION_ANGLE 0.01f

bool snail_drawing(Draw_data* data) {
    Color cursor_color = data->colorized ? COLORIZED_CURSOR_COLOR : CURSOR_COLOR;
    int center_x = data->window_w / 2;
    int center_y = data->window_h / 2;
    int radius = min(data->window_w, data->window_h) / 2;

    float step = ((float) (M_PI * 2)) / ((float) data->current_array_len);
    for(int i = 0; i < data->current_array_len; i++) {
        float ratio = ((float) data->array[i]) / ((float) data->array_len);
        Color color;

        if(i == data->cursor)
            color = cursor_color;
        else if(data->colorized)
            color = get_color_distinct(ratio);
        else
            color = NO_COLOR;

        float ratio_radius = maxf(((float) radius) * ratio, 1);
        float angle1 = step * ((float) i) - CORRECTION_ANGLE;
        float angle2 = step * ((float) (i + 1)) + CORRECTION_ANGLE;
        int x1 = ratio_radius * cosf(angle1);
        int y1 = ratio_radius * sinf(angle1);
        int x2 = ratio_radius * cosf(angle2);
        int y2 = ratio_radius * sinf(angle2);

        if(!triangle(data, center_x, center_y, center_x + x1, center_y + y1, center_x + x2, center_y + y2, color))
            return false;
    }

    return true;
}
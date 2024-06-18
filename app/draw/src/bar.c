#include <stdlib.h>
#include <math.h>
#include "draw.h"

bool bar_drawing(Draw_data* data) {
    Color cursor_color = data->colorized ? COLORIZED_CURSOR_COLOR : CURSOR_COLOR;
    int w_unit = max(data->window_w / data->current_array_len, 1);
    int w_offset = max((data->window_w - w_unit * data->current_array_len) / 2, 0);

    for(int i = 0; i < data->current_array_len; i++) {
        float ratio = ((float) data->array[i]) / ((float) data->array_len);
        Color color;

        if(i == data->cursor)
            color = cursor_color;
        else if(data->colorized)
            color = get_color_distinct(ratio);
        else
            color = NO_COLOR;

        int h = (int) round(((float) data->window_h) * ratio);
        if(!rectangle(data, w_offset + w_unit * i, data->window_h - h, w_unit, h, color))
            return false;
    }

    return true;
}
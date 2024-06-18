#include <stdlib.h>
#include <math.h>
#include "draw.h"

#define MIN_TRIANGLES 36

bool circle_drawing(Draw_data* data) {
    int center_x = data->window_w / 2;
    int center_y = data->window_h / 2;
    int radius = min(data->window_w, data->window_h) / 2;
    int nb_triangles = max(data->current_array_len, MIN_TRIANGLES);

    float step = ((float) (M_PI * 2)) / ((float) nb_triangles);
    for(int i = 0; i < nb_triangles; i++) {
        int current = (int) floor(((float) i) / ((float) nb_triangles) * ((float) data->current_array_len));
        float ratio = ((float) data->array[current]) / ((float) data->array_len);
        Color color = current == data->cursor ? COLORIZED_CURSOR_COLOR : get_color(ratio);

        float angle1 = step * ((float) i) - M_PI / 2;
        float angle2 = step * ((float) (i + 1)) - M_PI / 2;
        int x1 = radius * cosf(angle1);
        int y1 = radius * sinf(angle1);
        int x2 = radius * cosf(angle2);
        int y2 = radius * sinf(angle2);

        if(!triangle(data, center_x, center_y, center_x + x1, center_y + y1, center_x + x2, center_y + y2, color))
            return false;
    }

    return true;
}
#include <stdlib.h>
#include "draw.h"
#include "draw_functions.h"

const short COLOR_MODE_ALL = 0;
const short COLOR_MODE_DEFAULT = 1;
const short COLOR_MODE_COLORIZED = 2;

//////// Add new draw functions here ////////

const int DRAW_FUNCTIONS_LEN = 4;
const Draw_function_info DRAW_FUNCTIONS[] = {
    {
        bar_drawing,
        COLOR_MODE_ALL
    },
    {
        dot_drawing,
        COLOR_MODE_ALL
    },
    {
        circle_drawing,
        COLOR_MODE_COLORIZED
    },
    {
        snail_drawing,
        COLOR_MODE_ALL
    }
};
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "render.h"
#include "common_api.h"
#include "draw_api.h"

const Color NO_COLOR = {255, 255, 255, 255};
const Color CURSOR_COLOR = {255, 90, 0, 255};
const Color COLORIZED_CURSOR_COLOR = {255, 255, 255, 255};

Color get_color(float ratio) {
    Color color = {0, 0, 0, 255};
    float step = 1.f/6.f;

    if(ratio < step) {
        color.r = 255;
        color.g = (int) round(255.0 * ratio / step);
    } else if(ratio < step * 2) {
        color.r = (int) round(255.0 * (1 - (ratio - step) / step));
        color.g = 255;
    } else if(ratio < step * 3) {
        color.g = 255;
        color.b = (int) round(255.0 * (ratio - step * 2) / step);
    } else if(ratio < step * 4) {
        color.g = (int) round(255.0 * (1 - (ratio - step * 3) / step));
        color.b = 255;
    } else if(ratio < step * 5) {
        color.r = (int) round(255.0 * (ratio - step * 4) / step);
        color.b = 255;
    } else if(ratio <= 1.f) {
        color.r = 255;
        color.b = (int) round(255.0 * (1 - (ratio - step * 5) / step));
    } else
        color.r = 255;

    return color;
}

Color get_color_distinct(float ratio) {
    Color color = {0, 0, 0, 255};
    float a = 1.f/5.f;

    if(ratio < a) {
        color.r = 255;
        color.g = (int) round(255.0 * ratio / a);
    } else if(ratio < a * 2) {
        color.r = (int) round(255.0 * (1 - (ratio - a) / a));
        color.g = 255;
    } else if(ratio < a * 3) {
        color.g = 255;
        color.b = (int) round(255.0 * (ratio - a * 2) / a);
    } else if(ratio < a * 4) {
        color.g = (int) round(255.0 * (1 - (ratio - a * 3) / a));
        color.b = 255;
    } else if(ratio <= 1.f) {
        color.r = (int) round(255.0 * (ratio - a * 4) / a);
        color.b = 255;
    } else {
        color.r = 255;
        color.b = 255;
    }

    return color;
}

Color get_color_greyscale(float ratio) {
    Color color = {0, 0, 0, 255};

    if(ratio < 0.5f)
        color.r = between(0, (int) round(255.0f * ratio / 0.5f), 255);
    else if(ratio <= 1.f)
        color.r = between(0, (int) round(255.0f * (1.f - ratio) / 0.5f), 255);
    else
        color.r = 0;

    color.g = color.r;
    color.b = color.r;
    return color;
}

Color get_color_greyscale_distinct(float ratio) {
    Color color = {0, 0, 0, 255};
    color.r = min((int) round(255.0 * ratio), 255);
    color.g = color.r;
    color.b = color.r;
    return color;
}

bool rectangle(Draw_data* data, int x, int y, int w, int h, Color color) {
    SDL_Color c = {color.r, color.g, color.b, color.a};
    SDL_Rect rect = {x, y, w, h};
    return draw_rect((Render*) (data->_private), &rect, c);
}

bool triangle(Draw_data* data, int x1, int y1, int x2, int y2, int x3, int y3, Color color) {
    SDL_Color c = {color.r, color.g, color.b, color.a};
    SDL_Vertex v1 = {{x1, y1}, c, {1, 1}};
    SDL_Vertex v2 = {{x2, y2}, c, {1, 1}};
    SDL_Vertex v3 = {{x3, y3}, c, {1, 1}};
    return draw_triangle((Render*) (data->_private), v1, v2, v3, c);
}
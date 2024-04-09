#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL_ttf.h> // Fix Unknown TTF_Font

extern const short WINDOW_QUIT;
extern const short KEY_Q;
extern const short KEY_P;
extern const short KEY_I;
extern const short KEY_ARROW_UP;
extern const short KEY_ARROW_DOWN;
extern const short KEY_ARROW_LEFT;
extern const short KEY_ARROW_RIGHT;
extern const short MOUSE_WHEEL_UP;
extern const short MOUSE_WHEEL_DOWN;

extern const SDL_Color BLACK_COLOR;
extern const SDL_Color WHITE_COLOR;
extern const SDL_Color RED_COLOR;
extern const SDL_Color ORANGE_COLOR;

typedef struct Render {
    int w, h, framerate;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_bool fullscreen;
    short* events;
} Render;

Render* init_render(int w, int h, int framerate, SDL_bool fullscreen);
void destroy_render(Render* render);
const char* render_error();

void show_window(Render* render);
void get_window_size(Render* render, int* w, int* y);

void refresh(Render* render);
SDL_bool fill_background(Render* render, SDL_Color color);
SDL_bool draw_rect(Render* render, SDL_Rect* rect, SDL_Color color);
SDL_bool draw_text(Render* render, char* text, SDL_Rect* r, SDL_Color color);

void handle_events(Render* render);
SDL_bool do_quit(Render* render);
short was_pressed(Render* render, short key);

#endif
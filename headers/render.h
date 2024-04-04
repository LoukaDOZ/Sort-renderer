#ifndef RENDER_H
#define RENDER_H

extern const short KEY_Q;
extern const short KEY_P;

extern const SDL_Color BLACK_COLOR;
extern const SDL_Color WHITE_COLOR;
extern const SDL_Color RED_COLOR;
extern const SDL_Color ORANGE_COLOR;

typedef struct Render Render;

Render* init_render(int w, int h, int framerate, SDL_bool fullscreen);
void destroy_render(Render* render);
const char* render_error();

void show_window(Render* render);
void get_window_size(Render* render, int* w, int* y);

void tick(Render* render);
void refresh(Render* render);
SDL_bool fill_background(Render* render, SDL_Color color);
SDL_bool draw_rect(Render* render, SDL_Rect* rect, SDL_Color color);
SDL_bool draw_text(Render* render, char* text, SDL_Rect* r, SDL_Color color);

void handle_events(Render* render);
SDL_bool do_quit(Render* render);
short was_pressed(Render* render, short key);

#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "render.h"

#define NB_EVENTS 2
#define QUIT_EVENTS 0

const short KEY_Q = 1;
const short KEY_P = 2;

const SDL_Color BLACK_COLOR = {0, 0, 0, 255};
const SDL_Color WHITE_COLOR = {255, 255, 255, 255};
const SDL_Color RED_COLOR = {255, 0, 0, 255};
const SDL_Color ORANGE_COLOR = {255, 90, 0, 255};

struct Render {
    int w, h, framerate;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_bool fullscreen;
    short* events;
};

//////// Render ////////

Render* init_render(int w, int h, int framerate, SDL_bool fullscreen) {
    Render* render = (Render*) malloc(sizeof(Render));
    if(render == NULL)
        return NULL;

    render->events = (short*) calloc(NB_EVENTS, sizeof(short));
    if(render->events == NULL) {
        destroy_render(render);
        return NULL;
    }

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0)
        return NULL;

    if(TTF_Init() != 0) {
        destroy_render(NULL);
        return NULL;
    }

    render->font = TTF_OpenFont("fonts/Roboto-Regular.ttf", 25);
    if(render->font == NULL) {
        destroy_render(render);
        return NULL;
    }

    render->window = SDL_CreateWindow("Sort renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_HIDDEN);
    if(render->window == NULL) {
        destroy_render(render);
        return NULL;
    }

    render->renderer = SDL_CreateRenderer(render->window, -1, 0);
    if(render->renderer == NULL) {
        destroy_render(render);
        return NULL;
    }

    render->w = w;
    render->h = h;
    render->framerate = framerate;
    render->fullscreen = fullscreen;
    return render;
}

void destroy_render(Render* render) {
    if(render != NULL) {
        if(render->renderer != NULL)
            SDL_DestroyRenderer(render->renderer);

        if(render->window != NULL)
            SDL_DestroyWindow(render->window);

        if(render->font != NULL)
            TTF_CloseFont(render->font);

        if(render->events != NULL)
            free(render->events);

        free(render);
    }

    TTF_Quit();
    SDL_Quit();
}

const char* render_error() {
    const char* sdl_error = SDL_GetError();

    if(strlen(sdl_error) > 0)
        return sdl_error;

    return TTF_GetError();
}

//////// Window ////////

void show_window(Render* render) {
    SDL_ShowWindow(render->window);
    SDL_RaiseWindow(render->window);
}

void get_window_size(Render* render, int* w, int* h) {
    SDL_GetWindowSize(render->window, w, h);
}

//////// Draw ////////

void tick(Render* render) {
    SDL_Delay((int) (1000 / render->framerate));
}

void refresh(Render* render) {
    SDL_RenderPresent(render->renderer);
}

SDL_bool fill_background(Render* render, SDL_Color color) {
    if(SDL_SetRenderDrawColor(render->renderer, color.r, color.g, color.b, color.a) != 0
            || SDL_RenderClear(render->renderer) != 0) {
        return SDL_FALSE;
    }

    return SDL_TRUE;
}

SDL_bool draw_rect(Render* render, SDL_Rect* rect, SDL_Color color) {
    if(SDL_SetRenderDrawColor(render->renderer, color.r, color.g, color.b, color.a) != 0 
            || SDL_RenderFillRect(render->renderer, rect) != 0)
        return SDL_FALSE;

    return SDL_TRUE;
}

SDL_bool draw_text(Render* render, char* text, SDL_Rect* r, SDL_Color color) {
    SDL_Surface * surface = TTF_RenderUTF8_Solid(render->font, text, color);
    if(surface == NULL)
        return SDL_FALSE;

    SDL_Texture * texture = SDL_CreateTextureFromSurface(render->renderer, surface);
    if(texture == NULL)
        return SDL_FALSE;

    if(SDL_RenderCopy(render->renderer, texture, NULL, r) != 0)
        return SDL_FALSE;

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    return SDL_TRUE;
}

//////// Events ////////

void handle_events(Render* render) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                render->events[QUIT_EVENTS] = SDL_TRUE;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.scancode) {
                    case 4: // SDL_SCANCODE_Q not working
                        render->events[KEY_Q]++;
                        break;
                    case SDL_SCANCODE_P:
                        render->events[KEY_P]++;
                        break;
                }
                break;
        }
    }
}

SDL_bool do_quit(Render* render) {
    return render->events[QUIT_EVENTS];
}

short was_pressed(Render* render, short key) {
    short count = render->events[key];
    render->events[key] = 0;
    return count;
}
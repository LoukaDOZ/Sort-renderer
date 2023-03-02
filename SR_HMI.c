#include "SR_HMI.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <pthread.h>

typedef int SR_Graph;

typedef struct SR_Color {
    int r;
    int g;
    int b;
    int a;
} SR_Color;

typedef struct SR_ActionPanel {
    int x;
    int y;
    int w;
    int h;
} SR_ActionPanel;

typedef struct SR_DataPanel {
    int x;
    int y;
    int w;
    int h;
} SR_DataPanel;

typedef struct SR_GraphPanel {
    int x;
    int y;
    int w;
    int h;
    SR_Graph graph;
} SR_GraphPanel;

typedef struct SR_SDLInfo {
    SDL_Window* window;
    SDL_Surface* surface;
    SDL_Renderer* renderer;
    SDL_Event event;
} SR_SDLInfo;

typedef struct SR_WindowInfo {
    int w;
    int h;
} SR_WindowInfo;

typedef struct SR_HMI {
    SR_SDLInfo sdl_info;
    SR_WindowInfo window_info;
    SR_Array* array;
    pthread_t thread_render;
    char error_msg[128];
} SR_Renderer;

typedef struct SR_Index {
    int* index;
    SR_Color color;
} SR_Index;

typedef struct SR_Array {
    int* array;
    int size;
    int nb_indexes;
    SR_Index* indexes;
} SR_Array;

const SR_Graph STAIRS_BAR_GRAPH = 0;
const SR_Graph STAIRS_DOT_GRAPH = 1;
const SR_Graph TRIANGLE_BAR_GRAPH = 2;
const SR_Graph TRIANGLE_DOT_GRAPH = 3;

void SR_Close(SR_HMI* hmi) {
    SDL_DestroyWindow(hmi->sdl_info->window);
    SDL_DestroySurface(hmi->sdl_info->surface);
    SDL_DestroyRenderer(hmi->sdl_info->renderer);
    SDL_Quit();
    free(hmi->array->indexes);
    free(hmi->array);
    free(hmi);
}

char* SR_GetError(SR_HMI* hmi) {
    return hmi->error_msg;
}

int SR_Init(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
        fprintf(stderr, "Fatal error : initialisation failed\n %s\n", SDL_GetError()); 
        return 0;
    }

    return 1;
}

SR_HMI* SR_InitHMIRenderer()
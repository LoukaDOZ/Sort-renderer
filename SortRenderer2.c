#include "SortRenderer2.h"
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

typedef struct SR_Renderer {
    SDL_Window* window;
    SDL_Surface* surface;
    SDL_Renderer* renderer;
    SDL_Event event;
    pthread_t thread_id;
    int w_width;
    int w_height;
    SR_Graph graph;
} SR_Renderer;

typedef struct SR_Index {
    int* index;
    SR_Color* color;
} SR_Index;

typedef struct SR_Array {
    int* array;
    int size;
    int nb_indexes;
    SR_Index* indexes;
} SR_Array;

typedef struct SR_ThreadArg {
    SR_Renderer* renderer;
    SR_Array* array;
} SR_ThreadArg;

const SR_Graph STAIRS_BAR_GRAPH = 0;
const SR_Graph STAIRS_DOT_GRAPH = 1;
const SR_Graph TRIANGLE_BAR_GRAPH = 2;
const SR_Graph TRIANGLE_DOT_GRAPH = 3;

int SR_HelperArraySwapValues(int* array, int size, int index1, int index2) {
    if(index1 >= size || index2 >= size) {
        fprintf(stderr, "error swaping values : out ouf bound\n");
        return 0;
    }

    int tmp = array[index1];
    array[index1] = array[index2];
    array[index2] = tmp;

    return 1;
}

SR_Renderer* SR_CreateRenderer(void){
    SR_Renderer* renderer = (SR_Renderer*) malloc(sizeof(SR_Renderer));

    return renderer;
}

SR_Array* SR_CreateArray(int* array, int size){
    SR_Array* a = (SR_Array*) malloc(sizeof(SR_Array));
    a->array = array;
    a->size = size;

    return a;
}

SR_Color* SR_CreateColor(int r, int g, int b, int a){
    if(r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 || a < 0 || a > 255) {
        fprintf(stderr, "rgb and alpha must be between 0 and 255");
        return 0;
    }

    SR_Color* color = (SR_Color*) malloc(sizeof(SR_Color));
    color->r = r;
    color->g = g;
    color->b = b;
    color->a = a;

    return color;
}

int SR_ArraySize(SR_Array* array, int* size) {
    *size = array->size;

    return 1;
}

int SR_ArrayPointer(SR_Array* from, int* to){
    to = from->array;

    return 1;
}

int SR_ArraySwapValues(SR_Array* array, int index1, int index2) {
    if(index1 >= array->size || index2 >= array->size) {
        fprintf(stderr, "error swaping values : out ouf bound : i1 = %d i2 = %d size = %d\n", index1, index2, array->size);
        return 0;
    }

    int tmp = array->array[index1];
    array->array[index1] = array->array[index2];
    array->array[index2] = tmp;

    return 1;
}

int SR_ArrayNewIndexRenderer(SR_Array* array, int* index, SR_Color* color){
    array->nb_indexes++;

    if(array->indexes == NULL || array->nb_indexes == 0)
        array->indexes = (SR_Index*) malloc(sizeof(SR_Index));
    else
        array->indexes = (SR_Index*) realloc(array->indexes, sizeof(SR_Index) * array->nb_indexes);

    array->indexes[array->nb_indexes - 1].index = index;
    array->indexes[array->nb_indexes - 1].color = color;

    return 1;
}

int SR_ArrayResetIndexes(SR_Array* array){
    free(array->indexes);
    array->indexes = NULL;
    array->nb_indexes = 0;
    
    return 1;
}

int SR_Init(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
        fprintf(stderr, "error initializing SDL: %s\n", SDL_GetError()); 
        return 0;
    }

    return 1;
}

int SR_InitRenderer(SR_Renderer* renderer) {
    renderer->window = SDL_CreateWindow("Sort Renderer", SDL_WINDOWPOS_CENTERED, 
                                        SDL_WINDOWPOS_CENTERED, 1000, 500, SDL_WINDOW_RESIZABLE);
    renderer->surface = SDL_GetWindowSurface(renderer->window);
    renderer->renderer = SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED);

    //SDL_SetWindowFullscreen(renderer->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (SDL_GetRendererOutputSize(renderer->renderer, &(renderer->w_width), 
                                    &(renderer->w_height)) != 0) {
        fprintf(stderr, "SDL_GetRendererOutputSize failed: %s", SDL_GetError());
        return 0;
    }

    return 1;
}

int SR_RendererSetGraph(SR_Renderer* renderer, SR_Graph graph) {
    renderer->graph = graph;

    return 1;
}

int SR_RendererGetGraph(SR_Renderer* renderer, SR_Graph* graph) {
    *graph = renderer->graph;

    return 1;
}

int SR_GetMinMaxValue(SR_Array* array, int* min, int* max) {
    *max = array->array[0];
    *min = array->array[0];
    for(int i = 1; i < array->size; i++) {
        if(array->array[i] > *max)
            *max = array->array[i];
        else if(array->array[i] < *min)
            *min = array->array[i];
    }

    if(*max < 1) *max = 1;

    return 1;
}

int SR_FillRec(SR_Renderer* renderer, double x, double y, double w, double h) {
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer->renderer, &rect);

    return 1;
}

int SR_DrawStairsBarGraph(SR_Renderer* renderer, SR_Array* array) {
    SDL_SetRenderDrawColor(renderer->renderer, 255, 255, 255, 255);

    int min, max;
    SR_GetMinMaxValue(array, &min, &max);

    for(int i = 0; i < array->size; i++) {
        double ratio = (double) (array->array[i] - min) / (double) (max - min);
        int height = (int) (renderer->w_height * ratio);

        int width = (double) renderer->w_width / (double) array->size;
        if(width < 1) width = 1;

        int x = i * width;
        int y = renderer->w_height - height;

        SR_FillRec(renderer, x, y, width, height);
    }
}

int SR_DrawStairsDotGraph(SR_Renderer* renderer, SR_Array* array) {
    SDL_SetRenderDrawColor(renderer->renderer, 255, 255, 255, 255);

    int min, max;
    SR_GetMinMaxValue(array, &min, &max);

    for(int i = 0; i < array->size; i++) {
        double ratio = (double) (array->array[i] - min) / (double) (max - min);
        double h = (double) renderer->w_height / (double) array->size;
        int height = h > 0 && h < 1 ? 1 : (int) h;

        int width = (double) renderer->w_width / (double) array->size;
        if(width < 1) width = 1;

        int x = i * width;
        int y = (int) (renderer->w_height - renderer->w_height * ratio);

        SR_FillRec(renderer, x, y, width, height);
    }
}

int SR_DrawTriangleBarGraph(SR_Renderer* renderer, SR_Array* array) {
    SDL_SetRenderDrawColor(renderer->renderer, 255, 255, 255, 255);

    int min, max;
    SR_GetMinMaxValue(array, &min, &max);

    for(int i = 0; i < array->size; i++) {
        double ratio = (double) (array->array[i] - min) / (double) (max - min);
        int height = (int) (renderer->w_height * ratio);

        int width = (double) renderer->w_width / (double) array->size;
        if(width < 1) width = 1;

        int x = i * width;
        int y = (renderer->w_height - height) / 2;

        SR_FillRec(renderer, x, y, width, height);
    }
}
   

int SR_DrawTriangleDotGraph(SR_Renderer* renderer, SR_Array* array) {
    int min, max;
    SR_GetMinMaxValue(array, &min, &max);

    for(int i = 0; i < array->size; i++) {
        double ratio = (double) (array->array[i] - min) / (double) (max - min);
        double h = (double) renderer->w_height / (double) array->size;
        int height = h > 0 && h < 1 ? 1 : (int) h;
        
        int width = (double) renderer->w_width / (double) array->size;
        if(width < 1) width = 1;
        
        int bar_h = (int) (renderer->w_height * ratio);
        if(array->array[i] == min && min != 0) bar_h = height;
        int hide_bar_h = bar_h - 2 * height;
        if(hide_bar_h < 0) hide_bar_h = 0;

        int x = i * width;
        int y = (renderer->w_height - bar_h) / 2;

        SDL_SetRenderDrawColor(renderer->renderer, 255, 255, 255, 255);
        SR_FillRec(renderer, x, y, width, bar_h);
        SDL_SetRenderDrawColor(renderer->renderer, 0, 0, 0, 255);
        SR_FillRec(renderer, x, y + height, width, hide_bar_h);
    }
}

int SR_DrawIndexes(SR_Renderer* renderer, SR_Array* array) {
    if(array->nb_indexes == 0 || array->indexes == NULL)
        return 0;

    for(int i = 0; i < array->nb_indexes; i++) {
        int width = (double) renderer->w_width / (double) array->size;
        if(width < 1) width = 1;
        int x = *(array->indexes[i].index) * width;

        SDL_SetRenderDrawColor(renderer->renderer, array->indexes[i].color->r, 
                                                array->indexes[i].color->g, 
                                                array->indexes[i].color->b, 
                                                array->indexes[i].color->a);

        SR_FillRec(renderer, x, 0, width, renderer->w_height);
    }

    return 1;
}
   

void* SR_LoopRender(void* args) {
    SR_ThreadArg arg = *(SR_ThreadArg*) args;
    SR_Renderer* renderer = (SR_Renderer*) arg.renderer;
    SR_Array* array = (SR_Array*) arg.array;
    int error = 1, success = 0;

    while(1) {
        while (SDL_PollEvent(&renderer->event)) {
            if (renderer->event.type == SDL_WINDOWEVENT 
                && renderer->event.window.event == SDL_WINDOWEVENT_CLOSE) {
                if (SDL_GetWindowID(renderer->window) == renderer->event.window.windowID)
                    pthread_exit(&success);
            }
        }

        SDL_SetRenderDrawColor(renderer->renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer->renderer);
        SR_FillRec(renderer, 0, 0, renderer->w_width, renderer->w_height);

        if(renderer->graph == STAIRS_BAR_GRAPH) {
            SR_DrawStairsBarGraph(renderer, array);
        } else if(renderer->graph == STAIRS_DOT_GRAPH) {
            SR_DrawStairsDotGraph(renderer, array);
        } else if(renderer->graph == TRIANGLE_BAR_GRAPH) {
            SR_DrawTriangleBarGraph(renderer, array);
        } else if(renderer->graph == TRIANGLE_DOT_GRAPH) {
            SR_DrawTriangleDotGraph(renderer, array);
        } else {
            fprintf(stderr, "SR_Run error : unknown graph");
            renderer->event.type = SDL_QUIT;
            pthread_exit(&error);
        }

        SR_DrawIndexes(renderer, array);

        SDL_RenderPresent(renderer->renderer);
        SDL_Delay(50);
    }

            
    pthread_exit(&success);
}

int SR_Run(SR_Renderer* renderer, SR_Array* array) {
    SR_ThreadArg* arg = (SR_ThreadArg*) malloc(sizeof(SR_ThreadArg));

    arg->array = array;
    arg->renderer = renderer;
    if(pthread_create(&(renderer->thread_id), PTHREAD_CREATE_JOINABLE, SR_LoopRender, arg) == -1) {
        fprintf(stderr, "SR_Run thread creation error\n");
        return 0;
    }

    return 1;
}

int SR_QuitEventOccurred(SR_Renderer* renderer) {
    if (renderer->event.type == SDL_WINDOWEVENT 
        && renderer->event.window.event == SDL_WINDOWEVENT_CLOSE) {
        if (SDL_GetWindowID(renderer->window) == renderer->event.window.windowID) {
            return 1;
        }
    }

    return 0;
}

int SR_DestroyRenderer(SR_Renderer* renderer) {
    SDL_DestroyWindow(renderer->window);
    SDL_Quit();
    free(renderer);

    return 1;
}

int SR_DestroyArray(SR_Array* array) {
    free(array);

    return 1;
}

int SR_DestroyColor(SR_Color* color){
    free(color);

    return 1;
}
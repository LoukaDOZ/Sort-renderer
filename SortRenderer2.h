#ifndef SORT_RENDERER2_H
#define SORT_RENDERER2_H

typedef struct SR_Renderer SR_Renderer;
typedef struct SR_Array SR_Array;
typedef struct SR_Color SR_Color;
typedef int SR_Graph;

extern const SR_Graph STAIRS_BAR_GRAPH;
extern const SR_Graph STAIRS_DOT_GRAPH;
extern const SR_Graph TRIANGLE_BAR_GRAPH;
extern const SR_Graph TRIANGLE_DOT_GRAPH;

SR_Renderer* SR_CreateRenderer(void);
SR_Array* SR_CreateArray(int* array, int size);
SR_Color* SR_CreateColor(int r, int g, int b, int a);

int SR_ArraySize(SR_Array* array, int* size);
int SR_ArrayPointer(SR_Array* from, int* to);
int SR_ArraySwapValues(SR_Array* array, int index1, int index2);
int SR_ArrayNewIndexRenderer(SR_Array* array, int* index, SR_Color* color);
int SR_ArrayResetIndexes(SR_Array* array);

int SR_Init(void);
int SR_InitRenderer(SR_Renderer* renderer);
int SR_RendererSetGraph(SR_Renderer* renderer, SR_Graph graph);
int SR_RendererGetGraph(SR_Renderer* renderer, SR_Graph* graph);
int SR_Run(SR_Renderer* renderer, SR_Array* array);
int SR_DestroyRenderer(SR_Renderer* renderer);
int SR_DestroyArray(SR_Array* array);
int SR_DestroyColor(SR_Color* color);

int SR_QuitEventOccurred(SR_Renderer* renderer);

#endif
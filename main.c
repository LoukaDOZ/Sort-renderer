#include "SortRenderer2.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void slow_down(int time) {
    usleep(time * 10000);
}

void wait_pause(int time) {
    usleep(time * 1000000);
}

int suffle(SR_Renderer* renderer, SR_Array* sr_array, int* seed, int xtimes) {
    int size;
    int* array;
    SR_Color* red = SR_CreateColor(255, 0, 0, 255);

    SR_ArraySize(sr_array, &size);
    SR_ArrayPointer(sr_array, array);

    int j = 0;
    SR_ArrayNewIndexRenderer(sr_array, &j, red);

    for(int i = 0; i < xtimes; i++) {
        if(SR_QuitEventOccurred(renderer))
                return 0;

        for(; j < size; j++) {
            if(SR_QuitEventOccurred(renderer))
                return 0;

            SR_ArraySwapValues(sr_array, j, rand() % size);
            slow_down(2);
        }
    }

    for(int i = 0; i < size; i++) {
        seed[i] = array[i];
    }

    SR_ArrayResetIndexes(sr_array);
    return 1;
}

int suffle_from_seed(SR_Renderer* renderer, SR_Array* sr_array, int* seed) {
    int size;
    int* array;
    SR_Color* red = SR_CreateColor(255, 0, 0, 255);

    SR_ArraySize(sr_array, &size);
    SR_ArrayPointer(sr_array, array);

    int i = 0;
    SR_ArrayNewIndexRenderer(sr_array, &i, red);

    for(; i < size; i++) {
        if(SR_QuitEventOccurred(renderer))
            return 0;

        int j = i;
        for(; j < size; j++) {
            if(SR_QuitEventOccurred(renderer))
                return 0;

            if(array[j] == seed[i])
                break;
        }

        SR_ArraySwapValues(sr_array, i, j);
        slow_down(2);
    }

    SR_ArrayResetIndexes(sr_array);
    return 1;
}

int array_copy(int* to, int* from, int size) {
    for(int i = 0; i < size; i++) {
        to[i] = from[i];
    }

    return 1;
}

int bubble_sort(SR_Renderer* renderer, SR_Array* sr_array) {
    int size;
    int* array;
    SR_Color* red = SR_CreateColor(255, 0, 0, 255);

    SR_ArraySize(sr_array, &size);
    SR_ArrayPointer(sr_array, array);

    int i = size - 1, j = i;
    SR_ArrayNewIndexRenderer(sr_array, &i, red);
    SR_ArrayNewIndexRenderer(sr_array, &j, red);

    printf("-----\n");
    for(int p = 0; p < size; p++)
        printf("%d --\n",array[p]);

    int max;
    for(; i >= 0; i--) {
        if(SR_QuitEventOccurred(renderer))
            return 0;

        max = i;
        for(j = i; j >= 0; j--) {
            if(SR_QuitEventOccurred(renderer))
                return 0;

    array[max] = array[max];
    array[j] = array[j];
            if(array[max] < array[j])
                max = j;
        }

        slow_down(3);
        SR_ArraySwapValues(sr_array, i, max);
    }
    
    SR_ArrayResetIndexes(sr_array);
    return 1;
}

int merge_sort_recur(SR_Renderer* renderer, SR_Array* sr_array, int* array, int size, 
                    int parent_pos) {
    if(size != 1) {
        int mid = size / 2;
        int rest_mid = size - mid;

        if(!merge_sort_recur(renderer, sr_array, array, mid, parent_pos))
            return 0;
        if(!merge_sort_recur(renderer, sr_array, &array[mid], rest_mid, parent_pos + mid))
            return 0;
        
        int tabi[mid], tabj[rest_mid];
        array_copy(tabi, array, mid);
        array_copy(tabj, &array[mid], rest_mid);

        SR_Color* red = SR_CreateColor(255, 0, 0, 255);

        int i = 0, j = 0, k = 0;
        int pk = parent_pos;
        SR_ArrayNewIndexRenderer(sr_array, &pk, red);

        for(; i < mid && j < rest_mid; k++, pk++) {
            if(SR_QuitEventOccurred(renderer))
                return 0;

            if(tabi[i] <= tabj[j]) {
                array[k] = tabi[i];
                i++;
            } else {
                array[k] = tabj[j];
                j++;
            }

            slow_down(3);
        }

        for(; i < mid; i++, k++, pk++)
            array[k] = tabi[i];

        for(; j < mid; j++, k++, pk++)
            array[k] = tabj[j];
    }
    
    SR_ArrayResetIndexes(sr_array);
    return 1;
}

int merge_sort(SR_Renderer* renderer, SR_Array* sr_array) {  
    int size;
    int* array;

    SR_ArraySize(sr_array, &size);
    SR_ArrayPointer(sr_array, array);
    
    if(!merge_sort_recur(renderer, sr_array, array, size, 0))
        return 0;
    
    SR_ArrayResetIndexes(sr_array);
    return 1;
}

int cocktail_shaker_sort(SR_Renderer* renderer, SR_Array* sr_array) {
    int size;
    int* array;
    SR_Color* red = SR_CreateColor(255, 0, 0, 255);

    SR_ArraySize(sr_array, &size);
    SR_ArrayPointer(sr_array, array);

    int i = 0;
    SR_ArrayNewIndexRenderer(sr_array, &i, red);

    int left = 0;
    int right = size - 1;
    while(left <= right) {
        if(SR_QuitEventOccurred(renderer))
            return 0;

        int keep = left;
        for(i = left; i < right + 1; i++) {
            if(SR_QuitEventOccurred(renderer))
                return 0;

            if(array[keep] < array[i])
                keep = i;
                
            usleep(500);
        }

        SR_ArraySwapValues(sr_array, right, keep);
        right--;

        keep = right;
        for(i = right; i >= left; i--) {
            if(SR_QuitEventOccurred(renderer))
                return 0;

            if(array[keep] > array[i])
                keep = i;

            usleep(500);
        }

        SR_ArraySwapValues(sr_array, left, keep);
        left++;
    }
    
    SR_ArrayResetIndexes(sr_array);
    return 1;
}

int insertion_sort(SR_Renderer* renderer, SR_Array* sr_array) {
    int size;
    int* array;
    SR_Color* red = SR_CreateColor(255, 0, 0, 255);

    SR_ArraySize(sr_array, &size);
    SR_ArrayPointer(sr_array, array);

    int i = 0, j = 0, k = 0;
    SR_ArrayNewIndexRenderer(sr_array, &i, red);

    for(; i < size; i++) {
        if(SR_QuitEventOccurred(renderer))
            return 0;

        for(j = i, k = i - 1; j > 0 && k >= 0; j--, k--) {
            if(SR_QuitEventOccurred(renderer))
                return 0;

            if(array[k] < array[j])
                break;

            SR_ArraySwapValues(sr_array, k, j);
        }

        slow_down(3);
    }
    
    SR_ArrayResetIndexes(sr_array);
    return 1;
}

int main(void) {
    srand(time(NULL));
    int size = 500;
    int array[size];
    int seed[size];
    SR_Renderer* renderer = SR_CreateRenderer();
    SR_Array* sr_array = SR_CreateArray(array, size);
    SR_Color* red = SR_CreateColor(255, 0, 0, 255);

    if(!SR_Init() || !SR_InitRenderer(renderer))
        return EXIT_FAILURE;

    SR_RendererSetGraph(renderer, STAIRS_BAR_GRAPH);
    SR_ArrayPointer(sr_array, array);

    for(int i = -1 * (size / 2), j = 0; j < size; i++, j++) {
        array[j] = i * i;
    }

    SR_ArrayResetIndexes(sr_array);
    SR_Run(renderer, sr_array);
    wait_pause(2);

    if(!suffle(renderer, sr_array, seed, 1)) {
        SR_DestroyRenderer(renderer);
        SR_DestroyArray(sr_array);
        SR_DestroyColor(red);

        return EXIT_FAILURE;
    }

    if(!bubble_sort(renderer, sr_array)) {
        SR_DestroyRenderer(renderer);
        SR_DestroyArray(sr_array);
        SR_DestroyColor(red);

        return EXIT_FAILURE;
    }

    wait_pause(2);
    if(!suffle_from_seed(renderer, sr_array, seed)) {
        SR_DestroyRenderer(renderer);
        SR_DestroyArray(sr_array);
        SR_DestroyColor(red);

        return EXIT_FAILURE;
    }

    if(!insertion_sort(renderer, sr_array)) {
        SR_DestroyRenderer(renderer);
        SR_DestroyArray(sr_array);
        SR_DestroyColor(red);

        return EXIT_FAILURE;
    }

    wait_pause(2);
    if(!suffle_from_seed(renderer, sr_array, seed)) {
        SR_DestroyRenderer(renderer);
        SR_DestroyArray(sr_array);
        SR_DestroyColor(red);

        return EXIT_FAILURE;
    }

    if(!merge_sort(renderer, sr_array)) {
        SR_DestroyRenderer(renderer);
        SR_DestroyArray(sr_array);
        SR_DestroyColor(red);

        return EXIT_FAILURE;
    }

    wait_pause(2);
    if(!suffle_from_seed(renderer, sr_array, seed)) {
        SR_DestroyRenderer(renderer);
        SR_DestroyArray(sr_array);
        SR_DestroyColor(red);

        return EXIT_FAILURE;
    }
    wait_pause(2);

    if(!cocktail_shaker_sort(renderer, sr_array)) {
        SR_DestroyRenderer(renderer);
        SR_DestroyArray(sr_array);
        SR_DestroyColor(red);

        return EXIT_FAILURE;
    }

    wait_pause(2);
    SR_DestroyRenderer(renderer);
    SR_DestroyArray(sr_array);
    SR_DestroyColor(red);

    return EXIT_SUCCESS;
}

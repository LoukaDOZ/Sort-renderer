#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include "sorts.h"

#define SLEEP_SEC 2
#define ONE_SEC_MS 1000

long now(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * ONE_SEC_MS + tv.tv_usec / ONE_SEC_MS;
}

short run_miracle_sort(Data* data) {
    while(run(data)) {
        bool sorted = true;
        for(int i = 1; i < data->array_len && run(data); i++) {
            if(data->array[i - 1] > data->array[i])
                sorted = false;

            data->cursor = i;
            tick(data);
        }

        if(sorted)
            break;

        long start_time = now() + SLEEP_SEC * ONE_SEC_MS;
        while(now() < start_time && run(data))
            tick(data);
    }
    
    if(run(data))
        debug("That's a miracle !");

    return SORT_SUCCESS;
}
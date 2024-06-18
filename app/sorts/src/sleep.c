#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include "sorts.h"

#define WAIT_DURATION 1000 // 1s

typedef struct Thread_args {
    Data* data;
    pthread_mutex_t* mutex;
    pthread_mutex_t* cond_mutex;
    pthread_cond_t* cond;
    int* index;
    int* finished_count;
    int value;
} Thread_args;

static void* sleep_for(void* args) {
    Thread_args* targs = (Thread_args*) args;
    struct timespec sleep_time;
    struct timeval tv;
    long ms = WAIT_DURATION * targs->value;

    timespec_get(&sleep_time, TIME_UTC);
    
    long s = (ms / 1000);
    long ns = (ms % 1000) * 1000000;

    sleep_time.tv_sec = sleep_time.tv_sec + s;
    sleep_time.tv_nsec = (sleep_time.tv_nsec + ns) % 1000000;

    pthread_mutex_lock(targs->cond_mutex);
    int res = pthread_cond_timedwait(targs->cond, targs->cond_mutex, &sleep_time);
    pthread_mutex_unlock(targs->cond_mutex);

    pthread_mutex_lock(targs->mutex);

    if(res == ETIMEDOUT) {
        targs->data->array[*(targs->index)] = targs->value;
        (*(targs->index))++;
    }
    
    (*(targs->finished_count))++;
    pthread_mutex_unlock(targs->mutex);
    pthread_exit(NULL);
}

short run_sleep_sort(Data* data) {
    pthread_t tids[data->array_len];
    Thread_args targs[data->array_len];
    pthread_mutex_t mutex, cond_mutex;
    pthread_cond_t cond;
    int index = 0, current_count = 0, finished_count = 0;
    bool kill_all = false;

    if(pthread_mutex_init(&mutex, NULL) != 0)
        return SORT_FAILURE;

    if (pthread_mutex_init(&cond_mutex, NULL) != 0) {
        pthread_mutex_destroy(&mutex);
        return SORT_FAILURE;
    }

    if (pthread_cond_init(&cond, NULL) != 0) {
        pthread_mutex_destroy(&mutex);
        pthread_mutex_destroy(&cond_mutex);
        return SORT_FAILURE;
    }

    for(int i = 0; i < data->array_len && run(data); i++) {
        if(data->array[i] < 0) {
            debug_err("Sleep sort cannot handle negative numbers");
            pthread_mutex_destroy(&mutex);
            pthread_mutex_destroy(&cond_mutex);
            pthread_cond_destroy(&cond);
            return SORT_FAILURE;
        }

        targs[i].data = data;
        targs[i].mutex = &mutex;
        targs[i].cond_mutex = &cond_mutex;
        targs[i].cond = &cond;
        targs[i].index = &index;
        targs[i].finished_count = &finished_count;
        targs[i].value = data->array[i];
    }

    if(!run(data)) {
        pthread_mutex_destroy(&mutex);
        pthread_mutex_destroy(&cond_mutex);
        pthread_cond_destroy(&cond);
        return SORT_SUCCESS;
    }

    for(int i = 0; i < data->array_len; i++) {
        if(pthread_create(&tids[i], NULL, &sleep_for, (void*) &targs[i]) != 0) {
            pthread_cond_broadcast(&cond);

            for(int j = 0; j < i; j++)
                pthread_join(tids[j], NULL);

            pthread_mutex_destroy(&mutex);
            pthread_mutex_destroy(&cond_mutex);
            pthread_cond_destroy(&cond);
            return SORT_FAILURE;
        }

        pthread_mutex_lock(&mutex);

        if(!run(data)) {
            pthread_mutex_unlock(&mutex);
            kill_all = true;
            break;
        }

        pthread_mutex_unlock(&mutex);
    }

    if(kill_all)
        pthread_cond_broadcast(&cond);

    while(true) {
        pthread_mutex_lock(&mutex);

        if(current_count != finished_count) {
            current_count = finished_count;
            data->cursor = index - 1;
            tick(data);
        }
        
        if(finished_count >= data->array_len) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        if(!kill_all) {
            if(!run(data)) {
                kill_all = true;
                pthread_cond_broadcast(&cond);
            }
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&cond_mutex);
    pthread_cond_destroy(&cond);
    return SORT_SUCCESS;
}
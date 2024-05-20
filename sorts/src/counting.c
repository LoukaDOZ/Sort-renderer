#include <stdlib.h>
#include <stdbool.h>
#include "api.h"
#include "sorts.h"

typedef struct Count Count;
struct Count {
    Count* next;
    int value, count;
};

static Count* create_count(int value) {
    Count* count = (Count*) malloc(sizeof(Count));
    if(count == NULL)
        return NULL;

    count->next = NULL;
    count->value = value;
    count->count = 1;

    return count;
}

static Count* find_count(Count* start, int value) {
    Count* current = start;
    
    while(current != NULL) {
        if(current->value == value)
            return current;

        current = current->next;
    }

    return NULL;
}

static Count* add_count(Count* start, int value) {
    Count* found = find_count(start, value);
    if(found != NULL) {
        found->count++;
        return start;
    }

    Count* new = create_count(value);
    if(new == NULL)
        return NULL;

    if(start == NULL)
        return new;

    if(start->value >= value) {
        new->next = start;
        return new;
    }

    Count* current = start;
    while(current->next != NULL && current->next->value < value)
        current = current->next;

    new->next = current->next;
    current->next = new;
    return start;
}

static void free_counts(Count* start) {
    while(start != NULL) {
        Count* tmp = start->next;
        free(start);
        start = tmp;
    }
}

static Count* count(Data* data) {
    Count* counts = NULL;

    for(int i = 0; i < data->array_len && run(data); i++) {
        Count* res = add_count(counts, data->array[i]);

        if(res == NULL) {
            free_counts(counts);
            return NULL;
        }

        counts = res;
        data->cursor = i;
        tick(data);
    }

    Count* current = counts;
    int previous_count = 0;

    while(current != NULL && run(data)) {
        current->count += previous_count;
        previous_count = current->count;
        current = current->next;
    }

    return counts;
}

short run_counting_sort(Data* data) {
    Count* counts = count(data);
    if(counts == NULL)
        return SORT_FAILURE;

    int copy[data->array_len];
    for(int i = 0; i < data->array_len && run(data); i++)
        copy[i] = data->array[i];

    if(!run(data))
        return SORT_SUCCESS;

    for(int i = data->array_len - 1; i >= 0; i--) {
        Count* found = find_count(counts, copy[i]);
        data->array[found->count - 1] = copy[i];
        found->count--;

        data->cursor = found->count - 1;
        tick(data);
    }

    free_counts(counts);
    return SORT_SUCCESS;
}
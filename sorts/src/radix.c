#include <stdlib.h>
#include <stdbool.h>
#include "api.h"
#include "sorts.h"

#define NO_VALUE -1

typedef struct Work_element {
    int work_value, real_value;
} Work_element;

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

static Count* count(Data* data, Work_element* work_array) {
    Count* counts = NULL;

    for(int i = 0; i < data->array_len && run(data); i++) {
        Count* res = add_count(counts, work_array[i].work_value);

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

static bool counting_sort(Data* data, Work_element* work_array) {
    Count* counts = count(data, work_array);
    if(counts == NULL)
        return false;

    if(!run(data))
        return true;

    for(int i = data->array_len - 1; i >= 0 && run(data); i--) {
        Count* found = find_count(counts, work_array[i].work_value);
        data->array[found->count - 1] = work_array[i].real_value;
        found->count--;

        data->cursor = found->count - 1;
        tick(data);
    }

    free_counts(counts);
    return true;
}

static int get_digit(int value, int n) {
    for(int i = 0; i < n; i++)
        value = value / 10;

    return value <= 0 ? NO_VALUE : value % 10;
}

short run_radix_sort(Data* data) {
    int max = data->array[0];

    for(int i = 1; i < data->array_len; i++) {
        if(!run(data))
            return SORT_SUCCESS;

        if(data->array[i] > max)
            max = data->array[i];

        data->cursor = i;
        tick(data);
    }

    Work_element work_array[data->array_len];
    for(int i = 0, current_max = max; current_max > 0; i++, current_max /= 10) {
        for(int j = 0; j < data->array_len; j++) {
            if(!run(data))
                return SORT_SUCCESS;

            work_array[j].work_value = get_digit(data->array[j], i);
            work_array[j].real_value = data->array[j];
        }

        counting_sort(data, work_array);
    }

    return SORT_SUCCESS;
}
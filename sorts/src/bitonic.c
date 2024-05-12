#include <stdlib.h>
#include <stdbool.h>
#include "api.h"
#include "sorts.h"

static int get(Data* data, int* dummies, int i) {
    return i < data->array_len ? data->array[i] : dummies[i - data->array_len];
}

static void set(Data* data, int* dummies, int i, int value) {
    if(i < data->array_len)
        data->array[i] = value;
    else
        dummies[i - data->array_len] = value;
}

static void bitonic_merge(Data* data, int start, int len, int* dummies, bool ascending) {
    if(len <= 1 || !run(data))
        return;

    int mid = len / 2;

    for(int i = start; i < start + mid && run(data); i++) {
        int v_i = get(data, dummies, i);
        int v_mid_i = get(data, dummies, mid + i);

        if((ascending && v_i > v_mid_i) || (!ascending && v_i < v_mid_i)) {
            set(data, dummies, i, v_mid_i);
            set(data, dummies, mid + i, v_i);
        }

        data->cursor = i;
        tick(data);
    }

    bitonic_merge(data, start, mid, dummies, ascending);
    bitonic_merge(data, start + mid, len - mid, dummies, ascending);
}

static void bitonic_sort(Data* data, int start, int len, int* dummies, bool ascending) {
    if(len <= 1 || !run(data))
        return;

    int mid = len / 2;

    bitonic_sort(data, start, mid, dummies, true);
    bitonic_sort(data, start + mid, len - mid, dummies, false);
    bitonic_merge(data, start, len, dummies, ascending);
}

static int floor_power_of_2(int len) {
    len |= (len >> 1);
    len |= (len >> 2);
    len |= (len >> 4);
    len |= (len >> 8);
    len |= (len >> 16);
    return len - (len >> 1);
}

static int get_dummy_value(Data* data) {
    int max = data->array[0];

    for(int i = 1; i < data->array_len; i++) {
        if(data->array[i] > max)
            max = data->array[i];
    }

    return max + 1;
}

short run_bitonic_sort(Data* data) {
    int p2 = floor_power_of_2(data->array_len);

    if(p2 < data->array_len) {
        int dummy_value = get_dummy_value(data);
        int dummies_len = p2 * 2 - data->array_len;
        int dummies[dummies_len];
        debug_warn("Bitonic sort works with array length being a power of 2 : adding %d temporary dummies (rendering may be affected)", dummies_len);

        for(int i = 0; i < dummies_len; i++)
            dummies[i] = dummy_value;

        bitonic_sort(data, 0, data->array_len + dummies_len, dummies, true);
    } else
        bitonic_sort(data, 0, data->array_len, NULL, true);

    return SORT_SUCCESS;
}
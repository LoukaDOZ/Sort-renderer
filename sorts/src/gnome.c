#include <stdlib.h>
#include "sort.h"
#include "gnome.h"

short init_gnome_sort(Sort_info* info) {
    info->cursor = 1;
    return SORT_SUCCESS;
}

short gnome_sort(Sort_info* info) {
    if(info->cursor >= info->array_len)
        return SORT_FINISHED;

    if(info->array[info->cursor] < info->array[info->cursor - 1]) {
        swap(info, info->cursor, info->cursor - 1);
        info->cursor--;
    } else
        info->cursor++;

    return SORT_SUCCESS;
}

void free_gnome_sort(Sort_info* info) {
    // Ignore
}
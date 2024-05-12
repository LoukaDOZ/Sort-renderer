#include <stdlib.h>
#include "functions.h"
#include "sorts.h"

//////// Add new sorts here ////////

const int SORT_FUNCTIONS_LEN = 11;
const Sort_Algorithm SORT_ALGORITHMS[] = {
    {
        "Insertion sort",
        "Θ(n²)",
        &run_insertion_sort
    },
    {
        "Bubble sort",
        "Θ(n²)",
        &run_bubble_sort
    },
    {
        "Gnome sort",
        "Θ(n²)",
        &run_gnome_sort
    },
    {
        "Shaker sort",
        "Θ(n²)",
        &run_shaker_sort
    },
    {
        "Quick sort",
        "Θ(n log n)",
        &run_quick_sort
    },
    {
        "Heap sort",
        "Θ(n log n)",
        &run_heap_sort
    },
    {
        "Merge sort (Top-down)",
        "Θ(n log n)",
        &run_merge_top_down_sort
    },
    {
        "Merge sort (Bottom-up)",
        "Θ(n log n)",
        &run_merge_bottom_up_sort
    },
    {
        "Weave merge sort",
        "Θ(n²)",
        &run_weave_sort
    },
    {
        "Pancake sorting",
        "Θ(n²)",
        &run_pancake_sort
    },
    {
        "Bogo sort",
        "Θ((n + 1)!)",
        &run_bogo_sort
    },
    {
        "Stalin sort",
        "Θ(n)",
        &run_stalin_sort
    }
};

////////////////////////////////////
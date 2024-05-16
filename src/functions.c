#include <stdlib.h>
#include "functions.h"
#include "sorts.h"

//////// Add new sorts here ////////

const int SORT_FUNCTIONS_LEN = 21;
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
        "Selection sort",
        "Θ(n²)",
        &run_selection_sort
    },
    {
        "Cycle sort",
        "Θ(n²)",
        &run_cycle_sort
    },
    {
        "Odd-even sort",
        "Θ(n²)",
        &run_odd_even_sort
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
        "Bitonic sort",
        "Θ(log²(n))",
        &run_bitonic_sort
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
        "Timsort",
        "Θ(n log n)",
        &run_tim_sort
    },
    {
        "Sqrt block sort",
        "Θ(n log n)",
        &run_sqrt_block_sort
    },
    {
        "Patience sorting",
        "Θ(n log n)",
        &run_patience_sort
    },
    {
        "Pancake sorting",
        "Θ(n²)",
        &run_pancake_sort
    },
    {
        "Stooge sort",
        "Θ(n^2.7095)",
        &run_stooge_sort
    },
    {
        "Slow sort",
        "Θ(n^(log n / (2 + ε))) for ε > 0",
        &run_slow_sort
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
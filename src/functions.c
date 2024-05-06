#include <stdlib.h>
#include "functions.h"

//////// Add new sorts here ////////

#include "insertion.h"
#include "bubble.h"
#include "gnome.h"
#include "shaker.h"
#include "quick.h"
#include "merge_top_down.h"
#include "merge_bottom_up.h"
#include "bogo.h"
#include "stalin.h"

const int SORT_FUNCTIONS_LEN = 9;
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
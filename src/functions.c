#include <stdlib.h>
#include "functions.h"

//////// Add new sorts here ////////

#include "insertion.h"
#include "bubble.h"
#include "gnome.h"
#include "shaker.h"
#include "bogo.h"

const int SORT_FUNCTIONS_LEN = 5;
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
        "Bogo sort",
        "Θ((n + 1)!)",
        &run_bogo_sort
    }
};

////////////////////////////////////
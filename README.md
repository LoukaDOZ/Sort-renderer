# Sort-renderer
A C program to graphically show how different sorting algorithms work. Made with SDL2 library.


![Capture d’écran du 2024-04-14 15-04-27](https://github.com/LoukaDOZ/Sort-renderer/assets/46566140/4a29c943-0050-430a-9923-44d6112c4813)

## Usage

### Makefile

| Command          | Description                                 |
|------------------|---------------------------------------------|
| `make all`       | Build and run project (with default config) |
| `make install`   | Install SDL2 and SDL2 TTF libraries         |
| `make uninstall` | Uninstall SDL2 and SDL2 TTF libraries       |
| `make build`     | Compile project                             |
| `make run`       | Run project (with default config)           |
| `make clean`     | Clean compiled files                        |

### Manual run

```bash
make install
make build
./build/exec
```

Arguments :
| Argument                 | Description                                     | Type                           | Default        |
|--------------------------|-------------------------------------------------|--------------------------------|----------------|
| `-h, --help`             | Show help                                       |                                |                |
| `-w, --width <int>`      | Screen width in pixels                          | int (>= 2)                     | 500px          |
| `-e, --height <int>`     | Screen height in pixels                         | int (>= 2)                     | 500px          |
| `-r, --framerate <int>`  | Display max frames per seconds                  | int (>= 1 and <= 1000000)      | 60/s           |
| `-l, --looprate <int>`   | Simulation max loops per seconds                | int (>= 1 and <= 1000000)      | 500/s          |
| `-a, --array-size <int>` | Array size                                      | int (>= 2 and <= screen width) | Screen width   |
| `-s, --sort <int>`       | Start sort function modulo `SORT_FUNCTIONS_LEN` | int stating at 0               | 0 (first)      |
| `-f, --fullscreen`       | Set fullscreen                                  |                                | Not fullscreen |
| `-i, --no-info`          | Disable information messages                    |                                | Shown          |


### In game controls

| Control                         | Description                    |
|---------------------------------|--------------------------------|
| `Q`                             | Quit                           |
| `P`                             | Pause/resume simulation        |
| `I`                             | Show/hide information messages |
| `Left arrow`                    | Previous sort                  |
| `Right arrow`                   | Next sort                      |
| `Up arrow, mouse scrool up`     | Increase simulation looprate   |
| `Down arrow, mouse scrool down` | Decrease simulation looprate   |

## Implemented sorts

| Sort                   | Average complexity | Index |
|------------------------|--------------------|-------|
| Insertion sort         | Θ(n²)              | 0     |
| Bubble sort            | Θ(n²)              | 1     |
| Gnome sort             | Θ(n²)              | 2     |
| Shaker sort            | Θ(n²)              | 3     |
| Quick sort             | Θ(n log n)         | 4     |
| Heap sort              | Θ(n log n)         | 5     |
| Merge sort (Top-down)  | Θ(n log n)         | 6     |
| Merge sort (Bottom-up) | Θ(n log n)         | 7     |
| Bongo sort             | Θ((n + 1)!)        | 8     |
| Stalin sort            | Θ(n)               | 9     |

## Procedure to add a sort
### Define source file

Add the `SORT_NAME.c` source file in the `sorts/src/` folder and copy the following sample code (replace `SORT_NAME` by the sort algorithm name) :
```c
#include <stdlib.h>
#include "sort.h"
#include "SORT_NAME.h"

short init_SORT_NAME_sort(Sort_info* info) {
    // Init values and allocate needed memory
    return SORT_SUCCESS;
}

short SORT_NAME_sort(Sort_info* info) {
    // Run one iteration
    return SORT_SUCCESS;
}

void free_SORT_NAME_sort(Sort_info* info) {
    // Free potential manually allocated memory in init
}
```

:warning: You do not need to instantiate or free the Sort_info.

#### The Sort_info structure

```c
typedef struct Sort_info {
    int cursor, array_len, save_array_len;
    int* array;
    void* other;
} Sort_info;
```

| Field                  | Description                                                                                                                                         |
|------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------|
| `info->array_len`      | Array values length                                                                                                                                 |
| `info->array`          | Array values of `info->array_len` values                                                                                                            |
| `info->cursor`         | Defines "where" we are in the array at the current iteration. Mainly used for rendering but also useful to store a data                             |
| `info->other`          | Use it to store whatever other data needed. Must be allocated in the `init_SORT_NAME_sort` function and freed in the `free_SORT_NAME_sort` function |
| `info->save_array_len` | Should never be changed                                                                                                                             |

#### Return values

The `init_SORT_NAME_sort` and `SORT_NAME_sort` functions must return a value depending on the success or not of theses functions :
| Return value    | Functions             | Description                                           |
|-----------------|-----------------------|-------------------------------------------------------|
| `SORT_SUCCESS`  | Both                  | Everything is OK                                      |
| `SORT_FAILURE`  | Both                  | Something went wrong (unable to allocate memory, ...) |
| `SORT_FINISHED` | `SORT_NAME_sort` only | Sort function finished : the array should be sorted   |

### Define header file

Once the source file is ready, create a `SORT_NAME.h` (replace `SORT_NAME` by the sort algorithm name) file in `sorts/headers/` folder exposing the 3 functions defined previously :
```c
#ifndef SORT_NAME_H
#define SORT_NAME_H

short init_SORT_NAME_sort(Sort_info* info);
short SORT_NAME_sort(Sort_info* info);
void free_SORT_NAME_sort(Sort_info* info);

#endif
```

### The swap helper

The `sort.h` file describes a swap function to help swapping 2 array values :

```c
void swap(Sort_info* info, unsigned int i, unsigned int j)
```

:warning: Your are responsible for giving in bounds `i` and `j` indexes.

### Tell to the program to use the sort algorithm

In the `src/sort.c` file, update the following part of the code by adding the new sort information :
```c
#include "bubble.h"
#include "insertion.h"
#include "SORT_NAME.h" // Import the sort

const int SORT_FUNCTIONS_LEN = 3; // Update to match the length of SORT_FUNCTIONS
const Sort_function SORT_FUNCTIONS[] = {
    {
        "Insertion sort",
        "Θ(n²)",
        &init_insertion_sort,
        &insertion_sort,
        &free_insertion_sort
    },
    {
        "Bubble sort",
        "Θ(n²)",
        &init_bubble_sort,
        &bubble_sort,
        &free_bubble_sort
    },
    { // Add the sort
        "SORT_NAME sort", // Name
        "SORT_COMPLEXITY", // Average complexity
        &init_SORT_NAME_sort, // Init function reference
        &SORT_NAME_sort, // Sort function reference
        &free_SORT_NAME_sort // Free function reference
    }
};
```

#### Sort_function structure
```c
typedef struct Sort_function {
    char* name;
    char* complexity;
    short (*init)(Sort_info* info);
    short (*sort)(Sort_info* info);
    void (*free)(Sort_info* info);
} Sort_function;
```

# Example

Default configuration:
```bash
./build/exec
```

All arguments with a window size of 1000x700:
```bash
./build/exec -w 1000 -e 700 -r 30 -l 1000 -a 500 -s 2 -i
```

All arguments with fullscreen:
```bash
./build/exec -w 1000 -e 700 -r 30 -l 1000 -a 500 -s 2 -i -f
```

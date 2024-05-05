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
./bin/executable
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
### Define the source file

Add the `SORT_NAME.c` source file in the `sorts/src/` folder and copy the following sample code (replace `SORT_NAME` by the sort algorithm name) :
```c
#include <stdlib.h>
#include "api.h"
#include "SORT_NAME.h"

short run_SORT_NAME_sort(Data* data) {
    // Sort algorithm code
    return SORT_SUCCESS;
}
```
:warning: You do not need to instantiate or free the Data argument.

#### The Data structure

```c
typedef struct Data {
    int* array;
    int cursor, array_len;
    void* _private; // Do not touch
} Data;
```

| Field             | Description                                                                                                         |
|-------------------|---------------------------------------------------------------------------------------------------------------------|
| `data->array_len` | Array values length                                                                                                 |
| `data->array`     | Array of values to sort with a `data->array_len` length                                                             |
| `data->cursor`    | Defines "where" we are in the array at the current iteration. Used for rendering. Must be an index of `data->array` |
| `data->_private`  | Data for the simulation. Should not be touched                                                                      |

#### Return values

The `run_SORT_NAME_sort` function must return a `short` value depending on the success to run the algorithm :
| Return value   | Description                                             |
|----------------|---------------------------------------------------------|
| `SORT_SUCCESS` | Exit on success                                         |
| `SORT_FAILURE` | Exit on failure (unable to allocate memory for example) |

### The API

The `api.h` file describes helper functions for the sort algorithm :

```c
bool run(Data* data);
void tick(Data* data);
void swap(Data* data, int a, int b);
void debug(char* message, ...);
void debug_warn(char* message, ...);
void debug_err(char* message, ...);
```

| Function                         | Description                                                                                                                                              |
|----------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------|
| `run(Data* data)`                | Returns a boolean telling whether the algorithm should keep running or not. When false, the algorithm should exit                                        |
| `tick(Data* data)`               | This function is designed to be used after each iteration. It manages the looprate, when the simulation is paused or quitted, and it renders the changes |
| `swap(Data* data, int a, int b)` | Helps to swap to array values defined by their indexes `a` and `b`                                                                                       |
| `debug(char* message, ...)`      | Prints a log message. Works as printf with a new line                                                                                                    |
| `debug_warn(char* message, ...)` | Prints a warning message. Works as printf with a new line                                                                                                |
| `debug_err(char* message, ...)`  | Prints an error message. Works as printf with a new line                                                                                                 |

:warning: The simulation never checks for out of bounds.

### Define the header file

Once the source file is ready, create a `SORT_NAME.h` file in `sorts/headers/` folder exposing the previously defined function (replace `SORT_NAME` by the sort algorithm name) :
```c
#ifndef SORT_NAME_H
#define SORT_NAME_H

short run_SORT_NAME_sort(Data* data);

#endif
```

### Add the algorithm

In the `src/functions.c` file, update the following part of the code by adding the new sort information :
```c
#include <stdlib.h>
#include "functions.h"

//////// Add new sorts here ////////

#include "insertion.h"
#include "bubble.h"
#include "SORT_NAME.h" // Include the new sort header

const int SORT_FUNCTIONS_LEN = 3; // Update to match the length of SORT_ALGORITHMS
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
    { // Add the sort
        "SORT_NAME sort", // Name
        "SORT_COMPLEXITY", // Average complexity
        &run_SORT_NAME_sort // Sort function reference
    }
};

////////////////////////////////////
```

#### Sort_Algorithm structure
```c
typedef short (*Sort_function)(Data* data);

typedef struct Sort_Algorithm {
    char* name;
    char* complexity;
    Sort_function function;
} Sort_Algorithm;
```

# Example

Default configuration:
```bash
./bin/executable
```

All arguments with a window size of 1000x700 and starting with the 3rd sort :
```bash
./bin/executable -w 1000 -e 700 -r 30 -l 1000 -a 500 -s 2 -i
```

All arguments with fullscreen and starting with the 2nd sort from the end :
```bash
./bin/executable -w 1000 -e 700 -r 30 -l 1000 -a 500 -s -2 -i -f
```

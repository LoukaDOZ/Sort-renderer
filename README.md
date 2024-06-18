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
| Argument                 | Description                                                                                      | Type                                  | Default                    |
|--------------------------|--------------------------------------------------------------------------------------------------|---------------------------------------|----------------------------|
| `-h, --help`             | Show help                                                                                        |                                       |                            |
| `-w, --width <int>`      | Screen width in pixels                                                                           | int (>= 2)                            | 500px                      |
| `-e, --height <int>`     | Screen height in pixels                                                                          | int (>= 2)                            | 500px                      |
| `-r, --framerate <int>`  | Display max frames per seconds                                                                   | int (>= 1 and <= 1000000)             | 60/s                       |
| `-l, --looprate <int>`   | Simulation max loops per seconds                                                                 | int (>= 1 and <= 1000000)             | 500/s                      |
| `-a, --array-size <int>` | Array size                                                                                       | int (>= 3 and <= screen width)        | Screen width               |
| `-s, --sort <int>`       | Start sort function (modulo `SORT_ALGORITHMS_LEN`) (See [Implemented sorts](#implemented-sorts)) | int stating at 0                      | 0 (first)                  |
| `-d, --draw-mode <int>`  | Way to render the array (See [draw modes](#draw-modes))                                          | int (>= 0 and < `DRAW_FUNCTIONS_LEN`) | 0 (bar)                    |
| `-f, --fullscreen`       | Set fullscreen                                                                                   |                                       | Not fullscreen             |
| `-c, --colorized`        | Colorize display                                                                                 |                                       | All white                  |
| `-n, --manual-next-sort` | Disable launching next sort automatically                                                        |                                       | Automatic                  |
| `-m, --same-shuffle`     | Set the output array after shuffling to always be the same                                       |                                       | Different shuffling output |
| `-v, --no-validation`    | Disable validating the array is properly sorted after execution of an algorithm                  |                                       | Always validate            |
| `-i, --no-info`          | Disable information messages                                                                     |                                       | Shown                      |


### In game controls

| Control                         | Description                    |
|---------------------------------|--------------------------------|
| `Q`                             | Quit                           |
| `P`                             | Pause/resume simulation        |
| `R`                             | Restart current sort           |
| `I`                             | Show/hide information messages |
| `Left arrow`                    | Previous sort                  |
| `Right arrow`                   | Next sort                      |
| `Up arrow, mouse scrool up`     | Increase simulation looprate   |
| `Down arrow, mouse scrool down` | Decrease simulation looprate   |

## Implemented sorts

| Sort                   | Worst complexity                 | Average complexity                       | Best complexity                                       | Space complexity       | Index |
|------------------------|----------------------------------|------------------------------------------|-------------------------------------------------------|------------------------|-------|
| Insertion sort         | Θ(n²)                            | Θ(n²)                                    | Θ(n)                                                  | Θ(1)                   | 0     |
| Binary insertion sort  | Θ(n²)                            | Θ(n²)                                    | Θ(n log n)                                            | Θ(1)                   | 1     |
| Gnome sort             | Θ(n²)                            | Θ(n²)                                    | Θ(n)                                                  | Θ(1)                   | 2     |
| Bubble sort            | Θ(n²)                            | Θ(n²)                                    | Θ(n)                                                  | Θ(1)                   | 3     |
| Shaker sort            | Θ(n²)                            | Θ(n²)                                    | Θ(n)                                                  | Θ(1)                   | 4     |
| Selection sort         | Θ(n²)                            | Θ(n²)                                    | Θ(n²)                                                 | Θ(1)                   | 5     |
| Double selection sort  | Θ(n²)                            | Θ(n²)                                    | Θ(n)                                                  | Θ(1)                   | 6     |
| Exchange sort          | Θ(n²)                            | Θ(n²)                                    | Θ(n²)                                                 | Θ(1)                   | 7     |
| Shell sort             | Θ(n²)                            | Θ(n^1.25)                                | Θ(n log n)                                            | Θ(1)                   | 8     |
| Comb sort              | Θ(n²)                            | Θ(n^2 / 2^p) (p = number of increments)  | Θ(n log n)                                            | Θ(1)                   | 9     |
| Cycle sort             | Θ(n²)                            | Θ(n²)                                    | Θ(n²)                                                 | Θ(1)                   | 10    |
| Odd-even sort          | Θ(n²)                            | Θ(n²)                                    | Θ(n)                                                  | Θ(1)                   | 11    |
| Quick sort             | Θ(n²)                            | Θ(n log n)                               | Θ(n log n)                                            | Θ(log n)               | 12    |
| Heap sort              | Θ(n log n)                       | Θ(n log n)                               | If distinct keys : Θ(n log n) or if equal keys : Θ(n) | Θ(1)                   | 13    |
| Smooth sort            | Θ(n log n)                       | Θ(n log n)                               | Θ(n)                                                  | Θ(1)                   | 14    |
| Tournament sort        | Θ(n log n)                       | Θ(n log n)                               | Θ(n log n)                                            | Θ(n)                   | 15    |
| Bitonic sort           | Θ(log²(n))                       | Θ(log²(n))                               | Θ(log²(n))                                            | Θ(n log² n)            | 16    |
| Merge sort (Top-down)  | Θ(n log n)                       | Θ(n log n)                               | Θ(n log n)                                            | Θ(n)                   | 17    |
| Merge sort (Bottom-up) | Θ(n log n)                       | Θ(n log n)                               | Θ(n log n)                                            | Θ(n)                   | 18    |
| Weave merge sort       | Θ(n²)                            | Θ(n²)                                    | Θ(n²)                                                 | Θ(n)                   | 19    |
| Timsort                | Θ(n log n)                       | Θ(n log n)                               | Θ(n)                                                  | Θ(n)                   | 20    |
| Sqrt block sort        | Θ(n log n)                       | Θ(n log n)                               | Θ(n)                                                  | Θ(n)                   | 21    |
| Strand sort            | Θ(n²)                            | Θ(n²)                                    | Θ(n)                                                  | Θ(n)                   | 22    |
| Counting sort          | Θ(n + m) (m = count array)       | Θ(n + m) (m = count array)               | Θ(n + m) (m = count array)                            | Θ(m) (m = count array) | 23    |
| Radix sort             | Θ(l * (n + m)) (l = key length)  | Θ(l * (n + m)) (l = key length)          | Θ(l * (n + m)) (l = key length)                       | Θ(n + m)               | 24    |
| Patience sorting       | Θ(n log n)                       | Θ(n log n)                               | Θ(n)                                                  | Θ(n)                   | 25    |
| Pancake sorting        | Θ(n²)                            | Θ(n²)                                    | Θ(n)                                                  | Θ(1)                   | 26    |
| Stooge sort            | Θ(n^2.7095)                      | Θ(n^2.7095)                              | Θ(n^2.7095)                                           | Θ(n)                   | 27    |
| Slow sort              | Θ(n^(log n / (2 + ε))) for ε > 0 | Θ(n^(log n / (2 + ε))) for ε > 0         | Θ(n^(log n / (2 + ε))) for ε > 0                      | Θ(1)                   | 28    |
| Spaghetti sort         | Θ(n²)                            | Θ(n²)                                    | Θ(n²)                                                 | Θ(1)                   | 29    |
| Sleep sort             | Θ(n log n + max(input))          | Θ(n log n + max(input))                  | Θ(n log n + max(input))                               | Θ(n)                   | 30    |
| Stalin sort            | Θ(n)                             | Θ(n)                                     | Θ(n)                                                  | Θ(1)                   | 31    |
| Bogo sort              | ∞                                | Θ((n + 1)!)                              | Θ(n)                                                  | Θ(1)                   | 32    |
| Bozo sort              | ∞                                | Θ(n!)                                    | Θ(n)                                                  | Θ(1)                   | 33    |
| Miracle sort           | ∞                                | Θ(n * m) (m = expected number of checks) | Θ(n)                                                  | Θ(1)                   | 34    |

## Draw modes

| Draw   | Color mode       | Index |
|--------|------------------|-------|
| Bar    | All              | 0     |
| Dot    | All              | 1     |
| Circle | Always colorized | 2     |
| Snail  | All              | 3     |

# Example

Default configuration:
```bash
./bin/executable
```

All arguments with a window size of 1000x700, starting with the 3rd sort, and rendering with a dot draw mode :
```bash
./bin/executable -w 1000 -e 700 -r 30 -l 1000 -a 500 -d 1 -s 2 -m -i -c
```

All arguments with fullscreen, starting with the 2nd sort from the end, and rendering with a dot draw mode :
```bash
./bin/executable -w 1000 -e 700 -r 30 -l 1000 -a 500 -d 1 -s -2 -m -i -c -f
```

# Procedure to add a sort
## Define the source file

Add the `SORT_NAME.c` source file in the `app/sorts/src/` folder and copy the following sample code (replace `SORT_NAME` by the sort algorithm name) :
```c
#include <stdlib.h>
#include "sorts.h"

short run_SORT_NAME_sort(Data* data) {
    // Sort algorithm code
    return SORT_SUCCESS;
}
```
`sorts.h` auto includes [`sort_api.h`](#sort-api).

:warning: You do not need to instantiate or free the Data argument.

### Return values

The `run_SORT_NAME_sort` function must return a `short` value depending on the success to run the algorithm :
| Return value   | Description                                             |
|----------------|---------------------------------------------------------|
| `SORT_SUCCESS` | Exit on success                                         |
| `SORT_FAILURE` | Exit on failure (unable to allocate memory for example) |

## Expose the sort function

Once the source file is ready, open the `app/sorts/headers/sorts.h` file and expose the previously defined function (replace `SORT_NAME` by the sort algorithm name) :
```c
#ifndef SORTS_H
#define SORTS_H

#include "sort_api.h"

short run_insertion_sort(Data* data);
short run_bubble_sort(Data* data);
short run_SORT_NAME_sort(Data* data); // Expose the sort function

#endif
```

## Add the algorithm

In the `app/functions/src/sort_functions.c` file, update the following part of the code by adding the new sort information :
```c
#include <stdlib.h>
#include "sorts.h"
#include "sort_functions.h"

//////// Add new sorts here ////////

const int SORT_ALGORITHMS_LEN = 3; // Update to match the length of SORT_ALGORITHMS
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
```

### The Sort_Algorithm structure
```c
typedef short (*Sort_function)(Data* data);

typedef struct Sort_Algorithm {
    char* name;
    char* complexity;
    Sort_function function;
} Sort_Algorithm;
```

# Procedure to add a draw mode
## Define the source file

Add the `DRAW_NAME.c` source file in the `app/draw/src/` folder and copy the following sample code (replace `DRAW_NAME` by the drawing name) :
```c
#include <stdlib.h>
#include "draw.h"

bool DRAW_NAME_drawing(Draw_data* data) {
    // Draw function code
    return true;
}
```
`draw.h` auto includes [`draw_api.h`](#draw-api).

:warning: You do not need to instantiate or free the Draw_data argument.

### Return values

The `DRAW_NAME_drawing` function must return a `bool` value depending on the success (`true`) or the failure (`false`).

## Expose the draw function

Once the source file is ready, open the `app/draw/headers/draw.h` file and expose the previously defined function (replace `DRAW_NAME` by the drawing name) :
```c
#ifndef DRAW_H
#define DRAW_H

#include "draw_api.h"

bool bar_drawing(Draw_data* data);
bool dot_drawing(Draw_data* data);
bool circle_drawing(Draw_data* data);
bool snail_drawing(Draw_data* data);
bool DRAW_NAME_drawing(Draw_data* data); // Expose the draw function 
```

## Add the draw mode

In the `app/functions/src/draw_functions.c` file, update the following part of the code by adding the new sort information :
```c
#include <stdlib.h>
#include "draw.h"
#include "draw_functions.h"

const short COLOR_MODE_ALL = 0;
const short COLOR_MODE_DEFAULT = 1;
const short COLOR_MODE_COLORIZED = 2;

//////// Add new draw functions here ////////

const int DRAW_FUNCTIONS_LEN = 4;
const Draw_function_info DRAW_FUNCTIONS[] = {
    {
        bar_drawing,
        COLOR_MODE_ALL
    },
    {
        dot_drawing,
        COLOR_MODE_ALL
    },
    {
        circle_drawing,
        COLOR_MODE_COLORIZED
    },
    {
        snail_drawing,
        COLOR_MODE_ALL
    },
    { // Add the draw mode
        DRAW_NAME_drawing,
        COLOR_MODE_ALL /* Possible coloring :
                            COLOR_MODE_ALL :        normal and colorized
                            COLOR_MODE_DEFAULT :    normal only
                            COLOR_MODE_COLORIZED :  colorized only
                        */
    }
};
```

### The Draw_function_info structure
```c
typedef bool (*Draw_function)(Draw_data* data);

typedef struct Draw_function_info {
    Draw_function draw;
    short color_mode;
} Draw_function_info;
```

# Resolving conflicting function names

Some sort algorithms may declare functions with the same signature leading to compilation error, even when they are not declared in the header. To avoid this problem, mark "private" functions as static.

For example, `app/sorts/src/merge_top_down.c` and `app/sorts/src/merge_bottom_up.c` both use and declare a `static int* copy_elements(Data* data, int start, int len)`. The `static` keyword prevent conflicts when compiling.

# APIs
## Sort API
### Constants

| Constant       | Type  | Description          |
|----------------|-------|----------------------|
| `SORT_SUCCESS` | short | Exit sort on success |
| `SORT_FAILURE` | short | Exit sort on failure |

### Structures
#### Data structure

`sort_api.h` auto includes [`common_api.h`](#common-api).

```c
typedef struct Data {
    int* array;
    int cursor, array_len;
    void* _private; // Do not touch
} Data;
```

| Field       | Description                                                                                                                        |
|-------------|------------------------------------------------------------------------------------------------------------------------------------|
| `array_len` | Array length                                                                                                                       |
| `array`     | Array of integers to sort with an `array_len` length                                                                               |
| `cursor`    | Defines "where" we are in the array at the current iteration. Used for rendering. Must be an index between `0` and `array_len - 1` |
| `_private`  | Data for the simulation. Must not be touched                                                                                       |

### Functions

```c
bool run(Data* data);
void tick(Data* data);
void swap(Data* data, int a, int b);
```

| Function                         | Description                                                                                                                                              |
|----------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------|
| `run(Data* data)`                | Returns a boolean telling whether the algorithm should keep running or not. When false, the algorithm should exit                                        |
| `tick(Data* data)`               | This function is designed to be used after each iteration. It manages the looprate, when the simulation is paused or quitted, and it renders the changes |
| `swap(Data* data, int a, int b)` | Helps to swap to array values defined by their indexes `a` and `b`                                                                                       |

:warning: The simulation never checks for out of bounds.

## Draw API
### Constants

| Constant                 | Type  | Description                              |
|--------------------------|-------|------------------------------------------|
| `NO_COLOR`               | Color | Default rendering color if not colorized |
| `CURSOR_COLOR`           | Color | Default cursor color if not colorized    |
| `COLORIZED_CURSOR_COLOR` | Color | Cursor color colorized                   |

### Structures
#### Draw_data

`draw_api.h` auto includes [`common_api.h`](#common-api).

```c
typedef struct Draw_data {
    int* array;
    int array_len, current_array_len, cursor, window_w, window_h;
    bool colorized;
    void* _private; // Do not touch
} Draw_data;
```

| Field               | Description                                             |
|---------------------|---------------------------------------------------------|
| `array_len`         | Max array length                                        |
| `current_array_len` | Current array length                                    |
| `array`             | Copy of the array values of `array_len` length          |
| `cursor`            | Current cursor position between `0` and `array_len - 1` |
| `window_w`          | Window width                                            |
| `window_h`          | Window height                                           |
| `colorized`         | `true` if the drawing should be colorized               |
| `_private`          | Data for the rendering. Must not be touched             |

#### Color

```c
typedef struct Color {
    int r, g, b, a;
} Color;
```

| Field | Description                   |
|-------|-------------------------------|
| `r`   | Red value between 0 and 255   |
| `g`   | Green value between 0 and 255 |
| `b`   | Blue value between 0 and 255  |
| `a`   | Alpha value between 0 and 255 |

### Functions

```c
Color get_color(float ratio);
Color get_color_distinct(float ratio);

bool rectangle(Draw_data* data, int x, int y, int w, int h, Color color);
bool triangle(Draw_data* data, int x1, int y1, int x2, int y2, int x3, int y3, Color color);
```

| Function                                                                                 | Description                                                                             |
|------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------|
| `get_color(float ratio)`                                                                 | Returns a color gradien from red (0.0) to red (1.0) going though all colors             |
| `get_color_distinct(float ratio)`                                                        | Returns a color gradien from red (0.0) to purple (1.0) going though all distinct colors |
| `rectangle(Draw_data* data, int x, int y, int w, int h, Color color)`                    | Draws a rectangle                                                                       |
| `triangle(Draw_data* data, int x1, int y1, int x2, int y2, int x3, int y3, Color color)` | Draws a triangle                                                                        |

# Common API

`common_api.h` contains some helpful functions :

```c
int max(int a, int b);
int min(int a, int b);
int between(int inf, int sup, int v);

float maxf(float a, float b);
float minf(float a, float b);
float betweenf(float inf, float sup, float v);

void debug(char* message, ...);
void debug_warn(char* message, ...);
void debug_err(char* message, ...);
```

| Function                                  | Description                                                  |
|-------------------------------------------|--------------------------------------------------------------|
| `max(int a, int b)`                       | Returns the biggest value between `a` and `b`                |
| `min(int a, int b)`                       | Returns the smallest value between `a` and `b`               |
| `between(int inf, int sup, int v)`        | Returns `inf` if `v` < `inf`, `sup` if `v` > `sup`, else `v` |
| `maxf(float a, float b)`                  | Same as `max(int a, int b)` but for `float`                  |
| `minf(float a, float b)`                  | Same as `min(int a, int b)` but for `float`                  |
| `betweenf(float inf, float sup, float v)` | Same as `between(int inf, int sup, int v)` but for `float`   |
| `debug(char* message, ...)`               | Prints a log message. Works as printf with a new line        |
| `debug_warn(char* message, ...)`          | Prints a warning message. Works as printf with a new line    |
| `debug_err(char* message, ...)`           | Prints an error message. Works as printf with a new line     |

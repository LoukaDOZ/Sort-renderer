# Sort-renderer
A C program to graphically show how different sorting algorithms work. Made with SDL2 library.

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
| Argument                 | Description                      | Type                      | Default        |
|--------------------------|----------------------------------|---------------------------|----------------|
| `-h, --help`             | Show help                        |                           |                |
| `-w, --width <int>`      | Screen width in pixels           | int (>= 1)                | 500px          |
| `-e, --height <int>`     | Screen height in pixels          | int (>= 1)                | 500px          |
| `-r, --framerate <int>`  | Display max frames per seconds   | int (>= 1 and <= 1000000) | 60/s           |
| `-l, --looprate <int>`   | Simulation max loops per seconds | int (>= 1 and <= 1000000) | 500/s          |
| `-a, --array-size <int>` | Array size                       | int (>= 2)                | Screen width   |
| `-f, --fullscreen`       | Set fullscreen                   |                           | Not fullscreen |
| `-i, --no-info`          | Disable information messages     |                           | Shown          |


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

# Example

Default configuration:
```bash
./build/exec
```

All arguments with a window size of 1000x700:
```bash
./build/exec -w 1000 -e 700 -r 30 -l 1000 -a 500 -i
```

All arguments with fullscreen:
```bash
./build/exec -w 1000 -e 700 -r 30 -l 1000 -a 500 -i -f
```

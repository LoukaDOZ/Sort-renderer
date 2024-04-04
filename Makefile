CC=gcc
BuildFolder=./build
SortsFolder=./sorts
HeadersFolder=./headers
SortHeadersFolder=$(SortsFolder)/headers
SortSrcFolder=$(SortsFolder)/src
OFiles=$(patsubst %.c,%.o,$(wildcard $(SortSrcFolder)/*.c)) src/sort.o src/render.o src/main.o
Exec=exec
Flags=-lm -lpthread
SDLFlags=`sdl2-config --cflags --libs` -lSDL2 -lSDL2_ttf

all: build run

install:
	sudo apt install libsdl2-dev libsdl2-2.0-0 -y
	sudo apt install libfreetype6-dev libsdl2-ttf-dev libsdl2-ttf-2.0-0 -y

uninstall: clean
	sudo apt remove libsdl2-dev libsdl2-2.0-0 -y
	sudo apt remove libfreetype6-dev libsdl2-ttf-dev libsdl2-ttf-2.0-0 -y

build: $(OFiles)
	$(CC) -o $(BuildFolder)/$(Exec) $(BuildFolder)/*.o $(SDLFlags) $(Flags)

run:
	$(BuildFolder)/$(Exec)

%.o: %.c
	$(CC) -o $(BuildFolder)/$(notdir $@) -I $(HeadersFolder) -I $(SortHeadersFolder) -c $< $(SDLFlags) $(Flags)

clean:
	rm -f $(BuildFolder)/*.o $(BuildFolder)/$(Exec)

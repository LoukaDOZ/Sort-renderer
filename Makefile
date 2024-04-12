CC=gcc
BuildFolder=./build
SortsFolder=./sorts
SrcFolder=./src
HeadersFolder=./headers
SortHeadersFolder=$(SortsFolder)/headers
SortSrcFolder=$(SortsFolder)/src
OFiles=$(patsubst %.c,%.o,$(wildcard $(SortSrcFolder)/*.c)) $(patsubst %.c,%.o,$(wildcard $(SrcFolder)/*.c))
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
	$(CC) -g -o $(BuildFolder)/$(Exec) $(BuildFolder)/*.o $(SDLFlags) $(Flags)

run:
	$(BuildFolder)/$(Exec)

%.o: %.c
	$(CC) -g -o $(BuildFolder)/$(notdir $@) -I $(HeadersFolder) -I $(SortHeadersFolder) -c $< $(SDLFlags) $(Flags)

clean:
	rm -f $(BuildFolder)/*.o $(BuildFolder)/$(Exec)

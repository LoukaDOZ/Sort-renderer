CC=gcc
BinFolder=./bin
SortsFolder=./sorts
SrcFolder=./src
HeadersFolder=./headers
SortHeadersFolder=$(SortsFolder)/headers
SortSrcFolder=$(SortsFolder)/src
OFiles=$(patsubst %.c,%.o,$(wildcard $(SortSrcFolder)/*.c)) $(patsubst %.c,%.o,$(wildcard $(SrcFolder)/*.c))
Exec=executable
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
	$(CC) -o $(BinFolder)/$(Exec) $(BinFolder)/*.o $(SDLFlags) $(Flags)

run:
	$(BinFolder)/$(Exec)

%.o: %.c
	$(CC) -o $(BinFolder)/$(notdir $@) -I $(HeadersFolder) -I $(SortHeadersFolder) -c $< $(SDLFlags) $(Flags)

clean:
	rm -f $(BinFolder)/*.o $(BinFolder)/$(Exec)

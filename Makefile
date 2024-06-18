CC=gcc
BinFolder=./bin

Main=./app/main.c

RendererHeaders=./app/renderer/headers
RendererSrc=./app/renderer/src

SortsHeaders=./app/sorts/headers
SortsSrc=./app/sorts/src

FunctionsHeaders=./app/functions/headers
FunctionsSrc=./app/functions/src

APIsHeaders=./app/api/headers
APIsSrc=./app/api/src

DrawHeaders=./app/draw/headers
DrawSrc=./app/draw/src

OFiles=$(patsubst %.c,%.o,$(Main)) $(patsubst %.c,%.o,$(wildcard $(APIsSrc)/*.c)) $(patsubst %.c,%.o,$(wildcard $(SortsSrc)/*.c)) $(patsubst %.c,%.o,$(wildcard $(DrawSrc)/*.c)) $(patsubst %.c,%.o,$(wildcard $(FunctionsSrc)/*.c)) $(patsubst %.c,%.o,$(wildcard $(RendererSrc)/*.c))
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
	$(CC) -o $(BinFolder)/$(notdir $@) -I $(APIsHeaders) -I $(SortsHeaders) -I $(DrawHeaders) -I $(FunctionsHeaders) -I $(RendererHeaders) -c $< $(SDLFlags) $(Flags)

clean:
	rm -f $(BinFolder)/*.o $(BinFolder)/$(Exec)

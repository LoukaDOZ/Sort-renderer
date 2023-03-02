CC=gcc
CFLAGS=
LIBFLAGS=-lSDL2 -lpthread
EXEC=full

all : $(EXEC)

run : 
	./exec

renderer : SortRenderer2.o main.o
	$(CC) $(CFLAGS) -o exec $^ $(LIBFLAGS)

full : HMIManager.o SortsManager.o SortRenderer.o
	$(CC) $(CFLAGS) -o exec $^ $(LIBFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LIBFLAGS)


CC=gcc

CFLAGS= -pg -Wall -O3


TARGET=adam.out

all: $(TARGET)

$(TARGET): main.o
	$(CC) -pg -o $@ $^ -lm

main.o: main.c FloatVar.h
	$(CC) -c $(CFLAGS) main.c

clean:
	rm -rf *.o $(TARGET)

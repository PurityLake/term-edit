CC=gcc
EXECUTABLE=term-edit
CFLAGS=-Wall -pedantic -std=c11
INCLUDE=-I./include
LIBS=-lncurses

all:
	gcc $(CFLAGS) $(INCLUDE) -o $(EXECUTABLE) main.c $(LIBS)

.PHONY: clean
clean:
	rm -f $(EXECUTABLE)
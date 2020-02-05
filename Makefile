CC=gcc
EXECUTABLE=term-edit
CFLAGS=-Wall -pedantic -std=c11
INCLUDE=-I./include
LIBS=-lncurses
SRC_DIR=./src

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -o $@ -c $< 

all: line.o main.o
	$(CC) $(CFLAGS) $(INCLUDE) $(LIBS) -o $(EXECUTABLE) $^

.PHONY: clean
clean:
	rm -f $(EXECUTABLE)
	rm -f *.o
	rm -rf infer-out
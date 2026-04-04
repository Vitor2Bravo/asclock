# Make File Asclock C

BIN = casclock.exe
OBJ = main.o utils.o 
CC = gcc
CFLAGS = -Wall -ggdb
NODEBUG = -Wall

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

%.o: %.c
	$(CC) -c $< -o $@

build: 
	$(CC) -Wall -o $(BIN) main.c utils.c

clean:
	rm -f $(BIN) $(OBJ)

.PHONY: all clean

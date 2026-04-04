# Make File Asclock C

BIN = digiclock.exe
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
CPPFLAGS = -Iincludes/src
LDFLAGS = -Lincludes/src
LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -lcomdlg32

SRC = main.c
OBJ = $(SRC:.c=.o)

# Alvo padrão
all: build

# Configuração para Debug
debug: CFLAGS += -ggdb -O0
debug: $(BIN)

# Configuração para Release (Build)
build: CFLAGS += -O2 -s
build: $(BIN)

# Linkagem do executável
$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS) $(LDLIBS)

# Compilação dos arquivos objetos
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f $(BIN) *.o

.PHONY: all debug build clean

EXE = emu
CC = gcc
CFLAGS = -Wall
DEPS = test.h
OBJ = main.o test.o
SRC_DIR = src
OBJ_DIR = obj
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	    $(CC) $(CFLAGS) -c $< -o $@

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ -o $@

clean:
	$(RM) $(OBJ)

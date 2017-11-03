EXE = emu
CC = g++
CFLAGS = -Wall -std=c++11
SRC_DIR = src
OBJ_DIR = obj
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

all: $(EXE)

clean:
	$(RM) $(OBJ)

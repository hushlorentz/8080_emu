EXE = emu
CC = g++
CFLAGS = -Wall -std=c++11
DEPS = test.h
SRC_DIR = src
TEST_DIR = test
OBJ_DIR = obj
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run_tests: compile_tests

compile_tests: $(TEST_DIR)/test.cpp
	$(CC) $(CFLAGS) -c $< -o $@

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(OBJ)

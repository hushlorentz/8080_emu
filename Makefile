EXE = emu
TEST_EXE = run_tests
CC = g++
CFLAGS = -Wall -std=c++11
SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests
TEST_OBJ_DIR = $(TEST_DIR)/obj
OBJ = $(addprefix $(OBJ_DIR)/, disassembler.o io.o)
TEST_OBJ = $(addprefix $(TEST_OBJ_DIR)/, disassembler.o io.o)

$(EXE): $(OBJ) $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_EXE): $(TEST_OBJ) $(TEST_OBJ_DIR)/test.o
	$(CC) $(CFLAGS) $^ -o $(TEST_EXE)
	./$(TEST_EXE)

$(TEST_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	@ mkdir -p $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/test.o: $(TEST_DIR)/src/test.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o $(EXE) $(TEST_EXE)

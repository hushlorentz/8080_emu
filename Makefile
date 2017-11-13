EXE = emu
TEST_EXE = run_tests
CC = g++
CFLAGS = -Wall -std=c++11 -g
SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests
TEST_OBJ_DIR = $(TEST_DIR)/obj
OBJ = $(addprefix $(OBJ_DIR)/, bit_ops.o cpu.o disassembler.o io.o unhandled_op_code_exception.o)
TEST_OBJ = $(addprefix $(TEST_OBJ_DIR)/, bit_ops.o cpu.o disassembler.o io.o unhandled_op_code_exception.o)
TEST_SPECIFIC_OBJ = $(addprefix $(TEST_OBJ_DIR)/, accumulator.o bit_operations.o bootstrap.o data_transfer.o direct.o immediate.o op_codes.o pair_register.o rotate.o single_register.o)

$(EXE): $(OBJ) $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	@ mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

build_tests: $(TEST_OBJ) $(TEST_SPECIFIC_OBJ)
	$(CC) $(CFLAGS) $^ -o $(TEST_EXE)

$(TEST_EXE): build_tests
	./$(TEST_EXE)

$(TEST_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	@ mkdir -p $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/src/%.cpp
	@ mkdir -p $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o $(EXE) $(TEST_EXE)

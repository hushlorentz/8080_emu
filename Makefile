EXE = emu
TEST_EXE = run_tests
CC = g++
CFLAGS = -Wall -std=c++11 -g -F /Library/Frameworks
LFLAGS = -framework SDL2 -F /Library/Frameworks -I /Library/Frameworks/SDL2.framework/Headers
SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests
TEST_OBJ_DIR = $(TEST_DIR)/obj
OBJ = $(addprefix $(OBJ_DIR)/, bit_ops.o cabinet.o cpu.o io.o space_invaders.o unhandled_op_code_exception.o)
TEST_OBJ = $(addprefix $(TEST_OBJ_DIR)/, bit_ops.o cpu.o io.o space_invaders.o unhandled_op_code_exception.o)
TEST_SPECIFIC_OBJ = $(addprefix $(TEST_OBJ_DIR)/, accumulator.o bit_operations.o bootstrap.o call.o data_transfer.o direct.o immediate.o interrupts.o input_output.o jump.o operations.o op_codes.o pair_register.o port_handling.o return.o rotate.o single_register.o step.o)

$(EXE): $(OBJ) $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

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

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/src/%.cpp $(SRC_DIR)/cpu.cpp $(SRC_DIR)/space_invaders.cpp
	@ mkdir -p $(TEST_OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(TEST_OBJ_DIR)/*.o $(EXE) $(TEST_EXE)

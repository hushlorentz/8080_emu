#include "../../src/disassembler.h"

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

void disassemblerReturns1For0OpCode();

TEST_CASE("Disassembler returns 1 for NoOp") {
  unsigned char opCodes[] = {0};
  REQUIRE(processOpCode(opCodes, 0) == 1);
}


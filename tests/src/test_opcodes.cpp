#include "../../src/cpu.h"
#include "../../src/op_codes.h"
#include "../../src/unhandled_op_code_exception.h"

#include "catch.hpp"

using namespace Catch;

TEST_CASE("The CPU handles all the OpCodes correctly") {
  CPU c = CPU();
   
  SECTION("An unhandled OpCode throws an exception")
  {
    unsigned char program[1] = {0xFF};

    REQUIRE_THROWS_WITH(c.processProgram(program, 1), Contains("Unhandled Op Code!"));
  }

  SECTION("A program with only NOP, doesn't change the state of the CPU")
  {
    unsigned char program[1] = {NOP};

    c.processProgram(program, 1);

    REQUIRE(c.allClear());
  }

  SECTION("A new CPU instance is all clear")
  {
    REQUIRE(c.allClear());
  }

  SECTION("A program can set the Carry Bit")
  {
    unsigned char program[1] = {STC};

    c.processProgram(program, 1);

    REQUIRE(c.carryBit() == 1);
  }

  SECTION("A program can flip the Carry Bit")
  {
    unsigned char program[1] = {CMC};

    c.processProgram(program, 1);
    REQUIRE(c.carryBit() == 1);
  }

  SECTION("A program can flip the Carry Bit twice")
  {
    unsigned char program[1] = {CMC};

    c.processProgram(program, 1);
    REQUIRE(c.carryBit() == 1);
    c.processProgram(program, 1);
    REQUIRE(c.carryBit() == 0);
  }
}


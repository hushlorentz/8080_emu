#include "../../src/cpu.h"
#include "../../src/op_codes.h"
#include "../../src/unhandled_op_code_exception.h"

#include "catch.hpp"

using namespace Catch;

TEST_CASE("The CPU handles all the OpCodes correctly") {
  CPU cpu = CPU();
   
  SECTION("The CPU starts in a cleared state")
  {
    REQUIRE(cpu.allClear());
  }

  SECTION("An unhandled OpCode throws an exception")
  {
    uint8_t program[1] = {0xFF};

    REQUIRE_THROWS_WITH(cpu.processProgram(program, 1), Contains("Unhandled Op Code!"));
  }

  SECTION("A program with only NOP, doesn't change the state of the CPU")
  {
    uint8_t program[1] = {NOP};

    cpu.processProgram(program, 1);

    REQUIRE(cpu.allClear());
  }

  SECTION("A program can set the Carry Bit")
  {
    uint8_t program[1] = {STC};

    cpu.processProgram(program, 1);

    REQUIRE(cpu.carryBitSet());
  }

  SECTION("A program can flip the Carry Bit")
  {
    uint8_t program[1] = {CMC};

    cpu.processProgram(program, 1);
    REQUIRE(cpu.carryBitSet());
  }

  SECTION("A program can reset the Carry Bit by flipping it twice")
  {
    uint8_t program[2] = {CMC, CMC};

    cpu.processProgram(program, 2);
    REQUIRE(!cpu.carryBitSet());
  }

  SECTION("A program can increment the registers")
  {
    uint8_t program[8] = {INR_A, INR_B, INR_C, INR_D, INR_E, INR_H, INR_L, INR_M};

    cpu.processProgram(program, 8);
    REQUIRE(cpu.registerA == 1);
    REQUIRE(cpu.registerB == 1);
    REQUIRE(cpu.registerC == 1);
    REQUIRE(cpu.registerD == 1);
    REQUIRE(cpu.registerE == 1);
    REQUIRE(cpu.registerH == 1);
    REQUIRE(cpu.registerL == 1);
    REQUIRE(cpu.registerM() == 1);
  }

  SECTION("Incrementing an 8-bit register once does not set the parity flag")
  {
    uint8_t program[1] = {INR_B};

    cpu.processProgram(program, 1);
    REQUIRE(!cpu.parityBitSet());
  }

  SECTION("Incrementing an 8-bit register three times sets the parity flag")
  {
    uint8_t program[3] = {INR_B, INR_B, INR_B};

    cpu.processProgram(program, 3);
    REQUIRE(cpu.parityBitSet());
  }

  SECTION("Incrementing a 16-bit register once does not set the parity flag")
  {
    uint8_t program[1] = {INR_M};

    cpu.processProgram(program, 1);
    REQUIRE(!cpu.parityBitSet());
  }

  SECTION("Incrementing a 16-bit register three times sets the parity flag")
  {
    uint8_t program[3] = {INR_M, INR_M, INR_M};

    cpu.processProgram(program, 3);
    REQUIRE(cpu.parityBitSet());
  }

  SECTION("A program can decrement all registers")
  {
    uint8_t program[8] = {DCR_A, DCR_B, DCR_C, DCR_D, DCR_E, DCR_H, DCR_L, DCR_M};

    cpu.processProgram(program, 8);
    REQUIRE((int8_t)cpu.registerA == -1);
    REQUIRE((int8_t)cpu.registerB == -1);
    REQUIRE((int8_t)cpu.registerC == -1);
    REQUIRE((int8_t)cpu.registerD == -1);
    REQUIRE((int8_t)cpu.registerE == -1);
    REQUIRE((int8_t)cpu.registerH == -1);
    REQUIRE((int8_t)cpu.registerL == -1);
    REQUIRE((int8_t)cpu.registerM() == -1);
  }

  SECTION("A program can set the accumulator to its compliment")
  {
    uint8_t program[1] = {CMA};

    cpu.processProgram(program, 1);
    REQUIRE((int8_t)cpu.registerA == -1);
  }
}

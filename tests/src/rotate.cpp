#include "../../src/cpu.h"
#include "../../src/op_codes.h"

#include "catch.hpp"

using namespace Catch;

TEST_CASE("The CPU handles operations that rotate the bits in the accumulator")
{
  CPU cpu;

  SECTION("A program can rotate the bits in the accumulator left")
  {
    uint8_t program[1] = { RLC };
    cpu.registerA = 0xf2;

    cpu.loadProgram(program, 1);
    cpu.processProgram();

    REQUIRE(cpu.carryBitSet());
    REQUIRE(cpu.registerA == 0xe5);
  }

  SECTION("A program can rotate the bits in the accumulator right")
  {
    uint8_t program[1] = { RRC };
    cpu.registerA = 0xf2;

    cpu.loadProgram(program, 1);
    cpu.processProgram();

    REQUIRE(!cpu.carryBitSet());
    REQUIRE(cpu.registerA == 0x79);
  }

  SECTION("A program can rotate the bits in the accumulator left and consider the carry bit")
  {
    uint8_t program[1] = { RAL };
    cpu.registerA = 0xb5;

    cpu.loadProgram(program, 1);
    cpu.processProgram();

    REQUIRE(cpu.carryBitSet());
    REQUIRE(cpu.registerA == 0x6a);
  }

  SECTION("A program can rotate the bits in the accumulator left with the carry bit set")
  {
    uint8_t program[2] = { STC, RAL };
    cpu.registerA = 0x03;

    cpu.loadProgram(program, 2);
    cpu.processProgram();

    REQUIRE(!cpu.carryBitSet());
    REQUIRE(cpu.registerA == 0x07);
  }

  SECTION("A program can rotate the bits in the accumulator right and consider the carry bit")
  {
    uint8_t program[1] = { RAR };
    cpu.registerA = 0x6b;

    cpu.loadProgram(program, 1);
    cpu.processProgram();

    REQUIRE(cpu.carryBitSet());
    REQUIRE(cpu.registerA == 0x35);
  }

  SECTION("A program can rotate the bits in the accumulator right with the carry bit set")
  {
    uint8_t program[2] = { STC, RAR };
    cpu.registerA = 0x6a;

    cpu.loadProgram(program, 2);
    cpu.processProgram();

    REQUIRE(!cpu.carryBitSet());
    REQUIRE(cpu.registerA == 0xb5);
  }
}

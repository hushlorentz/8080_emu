#include "catch.hpp"

#include "../../src/cpu.h"
#include "../../src/op_codes.h"

using namespace Catch;

TEST_CASE("Testing jump op codes")
{
  CPU cpu;

  SECTION("A program can set the program counter to the value of HL")
  {
    uint8_t program[1] = { PCHL };
    cpu.registerH = 0x72;
    cpu.registerL = 0x1d;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.programCounter == program + (0x721d >> 3));
  }

  SECTION("A program can set the program counter to the value of the immediate data")
  {
    uint8_t program[3] = { JMP, 0x3b, 0x22 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == program + (0x223b >> 3));
  }
}

#include "../../src/cpu.h"
#include "../../src/op_codes.h"
#include "../../src/unhandled_op_code_exception.h"

#include "catch.hpp"

using namespace Catch;

TEST_CASE("The CPU handles opcodes for operations with direct memory instructions")
{
  CPU cpu = CPU();
   
  SECTION("A program can store the accumulator in the referenced memory")
  {
    uint8_t program[3] = { STA, 0xff, 0xaa };
    cpu.registerA = 0x93;

    cpu.processProgram(program, 3);

    REQUIRE(cpu.memory[0xaaff] == 0x93);
  }

  SECTION("A program can store the referenced memory location in the accumulator")
  {
    uint8_t program[3] = { LDA, 0xff, 0xaa };
    cpu.memory[0xaaff] = 0x93;

    cpu.processProgram(program, 3);

    REQUIRE(cpu.registerA == 0x93);
  }

  SECTION("A program can store the values of H and L in the referenced memory")
  {
    uint8_t program[3] = { SHLD, 0x24, 0x81 };
    cpu.registerH = 0xae;
    cpu.registerL = 0x29;

    cpu.processProgram(program, 3);

    REQUIRE(cpu.memory[0x8124] == 0x29);
    REQUIRE(cpu.memory[0x8125] == 0xae);
  }

  SECTION("A program can load referenced memory into registers H and L")
  {
    uint8_t program[3] = { LXLD, 0x01, 0x10 };
    cpu.memory[0x1001] = 0x05;
    cpu.memory[0x1002] = 0xe2;

    cpu.processProgram(program, 3);

    REQUIRE(cpu.registerH == 0xe2);
    REQUIRE(cpu.registerL == 0x05);
  }
}

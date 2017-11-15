#include "catch.hpp"

#include "../../src/cpu.h"
#include "../../src/op_codes.h"

#include <iostream>

using namespace Catch;

TEST_CASE("Testing call op codes")
{
  CPU cpu;

  SECTION("A program can load a subroutine")
  {
    uint8_t program[3] = { CALL, 0x3b, 0x08 };
    std::cout << static_cast<void *>(program);

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == program + (0x083b >> 3));
    REQUIRE(cpu.stackPointer == 0xfffe);
  }
}

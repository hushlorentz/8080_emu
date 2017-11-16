#include "catch.hpp"

#include "../../src/cpu.h"
#include "../../src/op_codes.h"

using namespace Catch;

TEST_CASE("Testing operational opcodes")
{
  CPU cpu;

  SECTION("A program can quit with the QUIT op code")
  {
    uint8_t program[3] = { QUIT, INR_B, DCR_D };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.allClear());
  }
}

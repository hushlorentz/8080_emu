#include "catch.hpp"

#include "../../src/cpu.h"
#include "../../src/op_codes.h"

using namespace Catch;

TEST_CASE("Testing interrupt op codes")
{
  CPU cpu;

  SECTION("Calling an interrupt sets the stack pointer to the interrupt's location in memory")
  {
    uint8_t program[3] = { INR_E, INR_E, QUIT };

    cpu.loadProgram(program, 3);
    cpu.processProgram();

    cpu.handleInterrupt(RST_0);
    
    REQUIRE(cpu.stackPointer == 0xfffe);
    REQUIRE(cpu.memory[0xffff] == 0x3);
    REQUIRE(cpu.memory[0xfffe] == 0);
  }
}

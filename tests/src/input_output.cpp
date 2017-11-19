#include "catch.hpp"

#include "../../src/cpu.h"
#include "../../src/op_codes.h"

using namespace Catch;

uint8_t byteFromOutput = 0;

uint8_t inputPortController(uint8_t portAddress)
{
  return portAddress;
}

void outputPortController(uint8_t portAddress, uint8_t value)
{
  byteFromOutput = value;
}

TEST_CASE("Testing input / output op codes")
{
  CPU cpu;

  SECTION("An exeternal function can write bytes to register A")
  {
    uint8_t program[2] = { IN, 0xab };

    cpu.setInputPortHandler(&inputPortController);
    cpu.loadProgram(program, 2);

    cpu.processProgram();

    REQUIRE(cpu.registerA == 0xab);
  }

  SECTION("An exception is thrown if no input port handler is hooked up")
  {
    uint8_t program[2] = { IN, 0 };

    cpu.loadProgram(program, 2);

    REQUIRE_THROWS_WITH(cpu.processProgram(), Contains("No input port handler attached!"));
  }

  SECTION("An exeternal function can read bytes from the CPU")
  {
    uint8_t program[4] = { INR_A, INR_A, OUT, 0 };

    cpu.setOutputPortHandler(&outputPortController);
    cpu.loadProgram(program, 4);

    cpu.processProgram();

    REQUIRE(byteFromOutput == cpu.registerA);
  }

  SECTION("An exception is thrown if no output port handler is hooked up")
  {
    uint8_t program[2] = { OUT, 0 };

    cpu.loadProgram(program, 2);

    REQUIRE_THROWS_WITH(cpu.processProgram(), Contains("No output port handler attached!"));
  }
}

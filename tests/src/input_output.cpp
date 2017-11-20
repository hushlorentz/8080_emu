#include "catch.hpp"

#include "../../src/cpu.h"
#include "../../src/op_codes.h"
#include "../../src/port_handler.h"

using namespace Catch;

class MockPortHandler : public PortHandler
{
  public:
    uint8_t value;

    virtual uint8_t inputPortHandler(uint8_t portAddress)
    {
      return portAddress;
    }

    virtual uint8_t outputPortHandler(uint8_t portAddress, uint8_t value)
    {
      this->value = value;
      return 3;
    }
};

TEST_CASE("Testing input / output op codes")
{
  CPU cpu;
  MockPortHandler portHandler;

  SECTION("An exeternal function can write bytes to register A")
  {
    uint8_t program[2] = { IN, 0xab };

    cpu.setPortHandler(&portHandler);
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

    cpu.setPortHandler(&portHandler);
    cpu.loadProgram(program, 4);

    cpu.processProgram();

    REQUIRE(portHandler.value == cpu.registerA);
  }

  SECTION("An exception is thrown if no output port handler is hooked up")
  {
    uint8_t program[2] = { OUT, 0 };

    cpu.loadProgram(program, 2);

    REQUIRE_THROWS_WITH(cpu.processProgram(), Contains("No output port handler attached!"));
  }
}

#include "../../src/cpu.h"
#include "../../src/op_codes.h"
#include "../../src/unhandled_op_code_exception.h"

#include "catch.hpp"

using namespace Catch;

TEST_CASE("The CPU can step through one instruction at a time")
{
  CPU cpu = CPU();
   
  SECTION("The CPU executes one instruction at a time if stepping through")
  {
    uint8_t program[4] = { INR_B, INR_B, INR_B, INR_B };

    cpu.stepThrough = true;
    cpu.loadProgram(program, 4);

    cpu.processProgram();
    REQUIRE(cpu.registerB == 0x01);

    cpu.processProgram();
    REQUIRE(cpu.registerB == 0x02);

    cpu.processProgram();
    REQUIRE(cpu.registerB == 0x03);

    cpu.processProgram();
    REQUIRE(cpu.registerB == 0x04);
  }

  SECTION("The CPU executes the rest of the program if stepping is disabled")
  {
    uint8_t program[4] = { INR_B, INR_B, INR_B, INR_B };

    cpu.stepThrough = true;
    cpu.loadProgram(program, 4);

    cpu.processProgram();
    REQUIRE(cpu.registerB == 0x01);

    cpu.processProgram();
    REQUIRE(cpu.registerB == 0x02);
    cpu.stepThrough = false;

    cpu.processProgram();
    REQUIRE(cpu.registerB == 0x04);
  }
}

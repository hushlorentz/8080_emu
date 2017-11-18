#include "catch.hpp"

#include "../../src/cpu.h"
#include "../../src/op_codes.h"

using namespace Catch;

TEST_CASE("Testing interrupt op codes")
{
  CPU cpu;

  SECTION("Calling an interrupt sets the stack pointer to the interrupt's location in memory")
  {
    uint8_t program[11] = { NOP, INR_D, QUIT, NOP, NOP, NOP, NOP, NOP, INR_C, INR_C, RET };

    cpu.stepThrough = true;
    cpu.loadProgram(program, 11);

    cpu.processProgram();
    cpu.handleInterrupt(RST_1);
    REQUIRE(cpu.stackPointer == 0xfffe);
    REQUIRE(cpu.memory[0xffff] == 0x1);
    REQUIRE(cpu.memory[0xfffe] == 0);

    cpu.processProgram();
    REQUIRE(cpu.registerC == 0x1);

    cpu.processProgram();
    REQUIRE(cpu.registerC == 0x2);
    REQUIRE(cpu.registerD == 0x0);

    cpu.stepThrough = false;

    cpu.processProgram();
    REQUIRE(cpu.registerD == 0x1);
    REQUIRE(cpu.registerC == 0x2);
  }

  SECTION("RST_2 runs the subroutine at address 0x10")
  {
    uint8_t program[3] = { NOP, NOP, QUIT };
    cpu.memory[0x10] = INR_E;
    cpu.memory[0x11] = RET;

    cpu.stepThrough = true;
    cpu.loadProgram(program, 3);

    cpu.processProgram();
    cpu.handleInterrupt(RST_2);

    cpu.stepThrough = false;

    cpu.processProgram();
    REQUIRE(cpu.registerE == 0x1);
  }

  SECTION("RST_3 runs the subroutine at address 0x18")
  {
    uint8_t program[3] = { NOP, NOP, QUIT };
    cpu.memory[0x18] = INR_A;
    cpu.memory[0x19] = RET;

    cpu.stepThrough = true;
    cpu.loadProgram(program, 3);

    cpu.processProgram();
    cpu.handleInterrupt(RST_3);

    cpu.stepThrough = false;

    cpu.processProgram();
    REQUIRE(cpu.registerA == 0x1);
  }

  SECTION("RST_4 runs the subroutine at address 0x20")
  {
    uint8_t program[3] = { NOP, NOP, QUIT };
    cpu.memory[0x20] = INR_B;
    cpu.memory[0x21] = RET;

    cpu.stepThrough = true;
    cpu.loadProgram(program, 3);

    cpu.processProgram();
    cpu.handleInterrupt(RST_4);

    cpu.stepThrough = false;

    cpu.processProgram();
    REQUIRE(cpu.registerB == 0x1);
  }

  SECTION("RST_5 runs the subroutine at address 0x28")
  {
    uint8_t program[3] = { NOP, NOP, QUIT };
    cpu.memory[0x28] = INR_C;
    cpu.memory[0x29] = RET;

    cpu.stepThrough = true;
    cpu.loadProgram(program, 3);

    cpu.processProgram();
    cpu.handleInterrupt(RST_5);

    cpu.stepThrough = false;

    cpu.processProgram();
    REQUIRE(cpu.registerC == 0x1);
  }

  SECTION("RST_6 runs the subroutine at address 0x30")
  {
    uint8_t program[3] = { NOP, NOP, QUIT };
    cpu.memory[0x30] = INR_D;
    cpu.memory[0x31] = RET;

    cpu.stepThrough = true;
    cpu.loadProgram(program, 3);

    cpu.processProgram();
    cpu.handleInterrupt(RST_6);

    cpu.stepThrough = false;

    cpu.processProgram();
    REQUIRE(cpu.registerD == 0x1);
  }

  SECTION("RST_7 runs the subroutine at address 0x38")
  {
    uint8_t program[3] = { NOP, NOP, QUIT };
    cpu.memory[0x38] = INR_E;
    cpu.memory[0x39] = RET;

    cpu.stepThrough = true;
    cpu.loadProgram(program, 3);

    cpu.processProgram();
    cpu.handleInterrupt(RST_7);

    cpu.stepThrough = false;

    cpu.processProgram();
    REQUIRE(cpu.registerE == 0x1);
  }

  SECTION("RST_0 sets the programCounter to address 0")
  {
    uint8_t program[4] = { NOP, NOP, NOP, QUIT };
    cpu.stepThrough = true;
    cpu.loadProgram(program, 4);

    cpu.processProgram();
    REQUIRE(cpu.programCounter == 1);

    cpu.processProgram();
    REQUIRE(cpu.programCounter == 2);

    cpu.processProgram();
    REQUIRE(cpu.programCounter == 3);

    cpu.handleInterrupt(RST_0);
    cpu.processProgram();
    REQUIRE(cpu.programCounter == 1);
  }
}

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

    REQUIRE(cpu.programCounter == 0x721d);
  }

  SECTION("A program can set the program counter to the value of the immediate data")
  {
    uint8_t program[3] = { JMP, 0x3b, 0x22 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == 0x223b);
  }

  SECTION("JC jumps if the carry bit is set")
  {
    uint8_t program[4] = { STC, JC, 0xe9, 0xa6 };

    cpu.processProgram(program, 4);

    REQUIRE(cpu.programCounter == 0xa6e9);
  }

  SECTION("JC does not jump if the carry bit is not set")
  {
    uint8_t program[3] = { JC, 0xe9, 0xa6 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == 3);
  }

  SECTION("JNC jumps if the carry bit is not set")
  {
    uint8_t program[3] = { JNC, 0xf4, 0x69 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == 0x69f4);
  }

  SECTION("JNC does not jump if the carry bit is set")
  {
    uint8_t program[4] = { STC, JNC, 0xff, 0xff };

    cpu.processProgram(program, 4);

    REQUIRE(cpu.programCounter == 4);
  }

  SECTION("JZ jumps if the zero bit is set")
  {
    uint8_t program[4] = { SBB_A, JZ, 0xaa, 0x11 };

    cpu.processProgram(program, 4);

    REQUIRE(cpu.programCounter == 0x11aa);
  }

  SECTION("JZ does not jump if the zero bit is not set")
  {
    uint8_t program[3] = { JZ, 0xff, 0xff };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == 3);
  }

  SECTION("JNZ jumps if the zero bit is not set")
  {
    uint8_t program[3] = { JNZ, 0x40, 0xd2 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == 0xd240);
  }

  SECTION("JNZ does not jump if the zero bit is set")
  {
    uint8_t program[4] = { SBB_A, JNZ, 0xff, 0xff };

    cpu.processProgram(program, 4);

    REQUIRE(cpu.programCounter == 4);
  }

  SECTION("JM jumps if the sign bit is set")
  {
    uint8_t program[4] = { DCR_A, JM, 0x30, 0x03 };

    cpu.processProgram(program, 4);

    REQUIRE(cpu.programCounter == 0x0330);
  }

  SECTION("JM does not jump if the sign bit is not set")
  {
    uint8_t program[3] = { JM, 0xff, 0xff };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == 3);
  }

  SECTION("JP jumps if the sign bit is not set")
  {
    uint8_t program[3] = { JP, 0xff, 0xff };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == 0xffff);
  }

  SECTION("JP does not jump if the sign bit is set")
  {
    uint8_t program[4] = { DCR_A, JP, 0xff, 0xff };

    cpu.processProgram(program, 4);

    REQUIRE(cpu.programCounter == 4);
  }

  SECTION("JPE jumps if the parity bit is set")
  {
    uint8_t program[6] = { INR_B, INR_B, INR_B, JPE, 0xad, 0xde };

    cpu.processProgram(program, 6);

    REQUIRE(cpu.programCounter == 0xdead);
  }

  SECTION("JPE does not jump if the parity bit is not set")
  {
    uint8_t program[3] = { JPE, 0xff, 0xff };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == 3);
  }

  SECTION("JPO jumps if the parity bit is not set")
  {
    uint8_t program[3] = { JPO, 0x34, 0x09 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == 0x0934);
  }

  SECTION("JPO does not jump if the parity bit is set")
  {
    uint8_t program[6] = { INR_D, INR_D, INR_D, JPO, 0xff, 0xff };

    cpu.processProgram(program, 6);

    REQUIRE(cpu.programCounter == 6);
  }
}

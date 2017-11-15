#include "catch.hpp"

#include "../../src/cpu.h"
#include "../../src/op_codes.h"

using namespace Catch;

TEST_CASE("Testing call op codes")
{
  CPU cpu;

  SECTION("A program can load a subroutine")
  {
    uint8_t program[3] = { CALL, 0x3b, 0x08 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == program + (0x083b >> 3));
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CC performs a call to a subroutine if the carry bit is set")
  {
    uint8_t program[4] = { STC, CC, 0x21, 0xe3 };

    cpu.processProgram(program, 4);

    REQUIRE(cpu.programCounter == program + (0xe321 >> 3));
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CC does not perform a call to a subroutine if the carry bit is not set")
  {
    uint8_t program[3] = { CC, 0x21, 0xe3 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == program + 3);
  }

  SECTION("CNC performs a call to a subroutine if the carry bit is not set")
  {
    uint8_t program[3] = { CNC, 0xaa, 0x10 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == program + (0x10aa >> 3));
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CNC does not perform a call to a subroutine if the carry bit is set")
  {
    uint8_t program[4] = { STC, CNC, 0x21, 0xe3 };

    cpu.processProgram(program, 4);

    REQUIRE(cpu.programCounter == program + 4);
  }

  SECTION("CZ performs a call to a subroutine if the zero bit is set")
  {
    uint8_t program[5] = { INR_C, DCR_C, CZ, 0x04, 0xc2 };

    cpu.processProgram(program, 5);

    REQUIRE(cpu.programCounter == program + (0xc204 >> 3));
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CZ does not perform a call to a subroutine if the zero bit is not set")
  {
    uint8_t program[3] = { CZ, 0x21, 0xe3 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == program + 3);
  }

  SECTION("CNZ performs a call to a subroutine if the zero bit is not set")
  {
    uint8_t program[3] = { CNZ, 0xa9, 0x9a };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == program + (0x9aa9 >> 3));
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CNZ does not perform a call to a subroutine if the zero bit is set")
  {
    uint8_t program[5] = { INR_C, DCR_C, CNZ, 0x04, 0xc2 };

    cpu.processProgram(program, 5);

    REQUIRE(cpu.programCounter == program + 5);
  }

  SECTION("CM performs a call to a subroutine if the sign bit is set")
  {
    uint8_t program[4] = { DCR_E, CM, 0xba, 0xab };

    cpu.processProgram(program, 4);

    REQUIRE(cpu.programCounter == program + (0xabba >> 3));
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CM does not perform a call to a subroutine if the sign bit is not set")
  {
    uint8_t program[3] = { CM, 0x04, 0xc2 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == program + 3);
  }

  SECTION("CP performs a call to a subroutine if the sign bit is not set")
  {
    uint8_t program[3] = { CP, 0xca, 0xca };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == program + (0xcaca >> 3));
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CP does not perform a call to a subroutine if the sign bit is set")
  {
    uint8_t program[4] = { DCR_H, CP, 0x04, 0xc2 };

    cpu.processProgram(program, 4);

    REQUIRE(cpu.programCounter == program + 4);
  }

  SECTION("CPE performs a call to a subroutine if the parity bit is set")
  {
    uint8_t program[6] = { INR_L, INR_L, INR_L, CPE, 0x32, 0xb1 };

    cpu.processProgram(program, 6);

    REQUIRE(cpu.programCounter == program + (0xb132 >> 3));
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CPE does not perform a call to a subroutine if the parity bit is not set")
  {
    uint8_t program[3] = { CPE, 0x04, 0xc2 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == program + 3);
  }

  SECTION("CPO performs a call to a subroutine if the parity bit is not set")
  {
    uint8_t program[3] = { CPO, 0x04, 0xc2 };

    cpu.processProgram(program, 3);

    REQUIRE(cpu.programCounter == program + (0xc204 >> 3));
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CPO does not perform a call to a subroutine if the parity bit is set")
  {
    uint8_t program[6] = { INR_L, INR_L, INR_L, CPO, 0x32, 0xb1 };

    cpu.processProgram(program, 6);

    REQUIRE(cpu.programCounter == program + 6);
  }
}

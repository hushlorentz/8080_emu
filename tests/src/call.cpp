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

    cpu.loadProgram(program, 3);
    cpu.processProgram();

    uint8_t mem = cpu.memory[0xffff];

    REQUIRE(cpu.programCounter == 0x083b);
    REQUIRE(cpu.stackPointer == 0xfffe);
    REQUIRE(mem == 3);
  }

  SECTION("CC performs a call to a subroutine if the carry bit is set")
  {
    uint8_t program[4] = { STC, CC, 0x21, 0xe3 };

    cpu.loadProgram(program, 4);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 0xe321);
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CC does not perform a call to a subroutine if the carry bit is not set")
  {
    uint8_t program[3] = { CC, 0x21, 0xe3 };

    cpu.loadProgram(program, 3);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 3);
  }

  SECTION("CNC performs a call to a subroutine if the carry bit is not set")
  {
    uint8_t program[3] = { CNC, 0xaa, 0x10 };

    cpu.loadProgram(program, 3);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 0x10aa);
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CNC does not perform a call to a subroutine if the carry bit is set")
  {
    uint8_t program[4] = { STC, CNC, 0x21, 0xe3 };

    cpu.loadProgram(program, 4);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 4);
  }

  SECTION("CZ performs a call to a subroutine if the zero bit is set")
  {
    uint8_t program[5] = { INR_C, DCR_C, CZ, 0x04, 0xc2 };

    cpu.loadProgram(program, 5);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 0xc204);
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CZ does not perform a call to a subroutine if the zero bit is not set")
  {
    uint8_t program[3] = { CZ, 0x21, 0xe3 };

    cpu.loadProgram(program, 3);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 3);
  }

  SECTION("CNZ performs a call to a subroutine if the zero bit is not set")
  {
    uint8_t program[3] = { CNZ, 0xa9, 0x9a };

    cpu.loadProgram(program, 3);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 0x9aa9);
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CNZ does not perform a call to a subroutine if the zero bit is set")
  {
    uint8_t program[5] = { INR_C, DCR_C, CNZ, 0x04, 0xc2 };

    cpu.loadProgram(program, 5);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 5);
  }

  SECTION("CM performs a call to a subroutine if the sign bit is set")
  {
    uint8_t program[4] = { DCR_E, CM, 0xba, 0xab };

    cpu.loadProgram(program, 4);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 0xabba);
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CM does not perform a call to a subroutine if the sign bit is not set")
  {
    uint8_t program[3] = { CM, 0x04, 0xc2 };

    cpu.loadProgram(program, 3);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 3);
  }

  SECTION("CP performs a call to a subroutine if the sign bit is not set")
  {
    uint8_t program[3] = { CP, 0xca, 0xca };

    cpu.loadProgram(program, 3);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 0xcaca);
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CP does not perform a call to a subroutine if the sign bit is set")
  {
    uint8_t program[4] = { DCR_H, CP, 0x04, 0xc2 };

    cpu.loadProgram(program, 4);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 4);
  }

  SECTION("CPE performs a call to a subroutine if the parity bit is set")
  {
    uint8_t program[6] = { INR_L, INR_L, INR_L, CPE, 0x32, 0xb1 };

    cpu.loadProgram(program, 6);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 0xb132);
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CPE does not perform a call to a subroutine if the parity bit is not set")
  {
    uint8_t program[3] = { CPE, 0x04, 0xc2 };

    cpu.loadProgram(program, 3);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 3);
  }

  SECTION("CPO performs a call to a subroutine if the parity bit is not set")
  {
    uint8_t program[3] = { CPO, 0x04, 0xc2 };

    cpu.loadProgram(program, 3);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 0xc204);
    REQUIRE(cpu.stackPointer == 0xfffe);
  }

  SECTION("CPO does not perform a call to a subroutine if the parity bit is set")
  {
    uint8_t program[6] = { INR_L, INR_L, INR_L, CPO, 0x32, 0xb1 };

    cpu.loadProgram(program, 6);
    cpu.processProgram();

    REQUIRE(cpu.programCounter == 6);
  }
}

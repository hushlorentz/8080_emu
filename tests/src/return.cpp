#include "catch.hpp"

#include "../../src/cpu.h"
#include "../../src/op_codes.h"

using namespace Catch;

TEST_CASE("Testing return op codes")
{
  CPU cpu;

  SECTION("A program can return from a subroutine")
  {
    uint8_t program[8] = { CALL, 0x04, 0x00, QUIT, INR_B, INR_B, INR_B, RET };

    cpu.loadProgram(program, 8);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0x10000);
    REQUIRE(cpu.registerB == 3);
    REQUIRE(!cpu.runProgram);
  }

  SECTION("RC returns if the carry flag is set")
  {
    uint8_t program[8] = { STC, CALL, 0x06, 0x00, INR_C, QUIT, INR_C, RC };

    cpu.loadProgram(program, 8);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0x10000);
    REQUIRE(cpu.registerC == 2);
    REQUIRE(!cpu.runProgram);
  }

  SECTION("RC does not return if the carry flag is not set")
  {
    uint8_t program[7] = { CALL, 0x05, 0x00, INR_C, QUIT, INR_C, RC };

    cpu.loadProgram(program, 7);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0xfffe);
    REQUIRE(cpu.registerC == 1);
    REQUIRE(cpu.runProgram);
  }

  SECTION("RNC performs a return if the carry bit is not set")
  {
    uint8_t program[7] = { CALL, 0x05, 0x00, INR_D, QUIT, INR_D, RNC };

    cpu.loadProgram(program, 7);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0x10000);
    REQUIRE(cpu.registerD == 2);
    REQUIRE(!cpu.runProgram);
  }

  SECTION("RNC does not perform a return if the carry bit is set")
  {
    uint8_t program[8] = { STC, CALL, 0x06, 0x00, INR_D, QUIT, INR_D, RNC };

    cpu.loadProgram(program, 8);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0xfffe);
    REQUIRE(cpu.registerD == 1);
    REQUIRE(cpu.runProgram);
  }

  SECTION("RZ performs a return if the zero bit is set")
  {
    uint8_t program[8] = { CALL, 0x05, 0x00, INR_E, QUIT, INR_E, DCR_E, RZ };

    cpu.loadProgram(program, 8);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0x10000);
    REQUIRE(cpu.registerE == 1);
    REQUIRE(!cpu.runProgram);
  }

  SECTION("RZ does not perform a return if the zero bit is not set")
  {
    uint8_t program[7] = { CALL, 0x05, 0x00, INR_E, QUIT, INR_E, RZ };

    cpu.loadProgram(program, 7);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0xfffe);
    REQUIRE(cpu.registerE == 1);
    REQUIRE(cpu.runProgram);
  }

  SECTION("RNZ performs a return if the zero bit is not set")
  {
    uint8_t program[7] = { CALL, 0x05, 0x00, INR_B, QUIT, INR_B, RNZ };

    cpu.loadProgram(program, 7);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0x10000);
    REQUIRE(cpu.registerB == 2);
    REQUIRE(!cpu.runProgram);
  }

  SECTION("RNZ does not perform a return if the zero bit is set")
  {
    uint8_t program[8] = { CALL, 0x05, 0x00, INR_E, QUIT, INR_E, DCR_E, RNZ };

    cpu.loadProgram(program, 8);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0xfffe);
    REQUIRE(cpu.registerE == 0);
    REQUIRE(cpu.runProgram);
  }

  SECTION("RM performs a return if the sign bit is set")
  {
    uint8_t program[7] = { CALL, 0x05, 0x00, INR_C, QUIT, DCR_C, RM };

    cpu.loadProgram(program, 7);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0x10000);
    REQUIRE(cpu.registerC == 0);
    REQUIRE(!cpu.runProgram);
  }

  SECTION("RM does not perform a return if the sign bit is not set")
  {
    uint8_t program[7] = { CALL, 0x05, 0x00, INR_C, QUIT, INR_C, RM };

    cpu.loadProgram(program, 7);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0xfffe);
    REQUIRE(cpu.registerC == 1);
    REQUIRE(cpu.runProgram);
  }

  SECTION("RP performs a return if the sign bit is not set")
  {
    uint8_t program[7] = { CALL, 0x05, 0x00, INR_D, QUIT, INR_D, RP };

    cpu.loadProgram(program, 7);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0x10000);
    REQUIRE(cpu.registerD == 2);
    REQUIRE(!cpu.runProgram);
  }

  SECTION("RP does not perform a return if the sign bit is set")
  {
    uint8_t program[7] = { CALL, 0x05, 0x00, INR_D, QUIT, DCR_D, RP };

    cpu.loadProgram(program, 7);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0xfffe);
    REQUIRE(cpu.registerD == 0xff);
    REQUIRE(cpu.runProgram);
  }

  SECTION("RPE performs a return if the parity bit is set")
  {
    uint8_t program[9] = { CALL, 0x05, 0x00, INR_E, QUIT, INR_E, INR_E, INR_E, RPE };

    cpu.loadProgram(program, 9);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0x10000);
    REQUIRE(cpu.registerE == 4);
    REQUIRE(!cpu.runProgram);
  }

  SECTION("RPE does not perform a return if the parity bit is not set")
  {
    uint8_t program[7] = { CALL, 0x05, 0x00, INR_E, QUIT, INR_E, RPE };

    cpu.loadProgram(program, 7);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0xfffe);
    REQUIRE(cpu.registerE == 0x01);
    REQUIRE(cpu.runProgram);
 }

  SECTION("RPO performs a return if the parity bit is not set")
  {
    uint8_t program[7] = { CALL, 0x05, 0x00, INR_B, QUIT, INR_B, RPO };

    cpu.loadProgram(program, 7);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0x10000);
    REQUIRE(cpu.registerB == 0x02);
    REQUIRE(!cpu.runProgram);
  }

  SECTION("RPO does not perform a return if the parity bit is set")
  {
    uint8_t program[9] = { CALL, 0x05, 0x00, INR_E, QUIT, INR_E, INR_E, INR_E, RPO };

    cpu.loadProgram(program, 9);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0xfffe);
    REQUIRE(cpu.registerE == 3);
    REQUIRE(cpu.runProgram);
  }
}

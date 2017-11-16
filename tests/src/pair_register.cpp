#include "../../src/cpu.h"
#include "../../src/op_codes.h"
#include "../../src/unhandled_op_code_exception.h"

#include "catch.hpp"

using namespace Catch;

TEST_CASE("The CPU handles opcodes for operations on register pairs correctly")
{
  CPU cpu = CPU();
   
  SECTION("A program can push data onto the stack")
  {
    uint8_t program[1] = { PUSH_B };
    cpu.registerB = 0x06;
    cpu.registerC = 0x15;
    cpu.stackPointer = 0xbbaa;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.memory[0xbbaa - 0x0001] == cpu.registerB);
    REQUIRE(cpu.memory[0xbbaa - 0x0002] == cpu.registerC);
  }

  SECTION("Register pair A uses the bits from the status register")
  {
    uint8_t program[2] = { STC, PUSH_PSW };
    cpu.registerA = 0xf5;
    cpu.stackPointer = 0x0045;

    cpu.processProgram(program, 2);

    REQUIRE(cpu.memory[0x0045 - 0x0001] == cpu.registerA);
    REQUIRE(cpu.memory[0x0045 - 0x0002] == cpu.status);
  }

  SECTION("A program can pop data off the stack and restore to register pairs")
  {
    uint8_t program[1] = { POP_D };
    cpu.memory[0x0a] = 0xe5;
    cpu.memory[0x09] = 0x11;
    cpu.stackPointer = 0x09;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerD == 0xe5);
    REQUIRE(cpu.registerE == 0x11);
  }

  SECTION("POP_PSW sets the status register directly")
  {
    uint8_t program[1] = { POP_PSW };
    cpu.memory[0x1a] = 0xff;
    cpu.memory[0x19] = 0xff;
    cpu.stackPointer = 0x19;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == 0xff);
    REQUIRE(cpu.status == 0xd7);
    REQUIRE(cpu.signBitSet());
    REQUIRE(cpu.carryBitSet());
    REQUIRE(cpu.auxiliaryCarryBitSet());
    REQUIRE(cpu.zeroBitSet());
    REQUIRE(cpu.parityBitSet());
  }

  SECTION("A program can add a 16-bit number spread across two registers")
  {
    uint8_t program[1] = { DAD_D };
    cpu.registerD = 0x33;
    cpu.registerE = 0x9f;
    cpu.registerH = 0xa1;
    cpu.registerL = 0x7b;
    uint16_t HLValue = cpu.registerH << 8 | cpu.registerL;
    uint16_t DEValue = cpu.registerD << 8 | cpu.registerE;
    uint16_t sum = HLValue + DEValue;

    cpu.processProgram(program, 1);

    REQUIRE(sum == 0xd51a);
    REQUIRE((cpu.registerH << 8 | cpu.registerL) == sum);
  }

  SECTION("A 16-bit addition with a carry sets the carry bit")
  {
    uint8_t program[1] = { DAD_B };
    cpu.registerB = 0xf0;
    cpu.registerC = 0x00;
    cpu.registerH = 0xf0;
    cpu.registerL = 0x00;
    uint16_t HLValue = cpu.registerH << 8 | cpu.registerL;
    uint16_t BCValue = cpu.registerB << 8 | cpu.registerC;
    uint16_t sum = HLValue + BCValue;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.carryBitSet());
    REQUIRE((cpu.registerH << 8 | cpu.registerL) == sum);
  }

  SECTION("DAD_SP uses the stack pointer as the operand")
  {
    uint8_t program[1] = { DAD_SP };
    cpu.stackPointer = 0x55a4;
    cpu.registerH = 0xc4;
    cpu.registerL = 0x88;
    uint16_t HLValue = cpu.registerH << 8 | cpu.registerL;
    uint16_t sum = HLValue + cpu.stackPointer;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.carryBitSet());
    REQUIRE((cpu.registerH << 8 | cpu.registerL) == sum);
  }

  SECTION("A program can increment a 16-bit register pair")
  {
    uint8_t program[1] = { INX_H };
    cpu.registerH = 0x0;
    cpu.registerL = 0xff;

    cpu.processProgram(program, 1);

    REQUIRE((cpu.registerH << 8 | cpu.registerL) == 0x0100);
  }

  SECTION("A program can increment the stack pointer")
  {
    uint8_t program[1] = { INX_SP };
    cpu.stackPointer = 0xffff;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.stackPointer == 0x10000);
  }

  SECTION("A program can decrement a 16-bit register pair")
  {
    uint8_t program[1] = { DCX_D };
    cpu.registerD = 0xee;
    cpu.registerE = 0xdd;

    cpu.processProgram(program, 1);

    REQUIRE((cpu.registerD << 8 | cpu.registerE) == 0xeedc);
  }

  SECTION("A program can decrement the stack pointer")
  {
    uint8_t program[1] = { DCX_SP };
    cpu.stackPointer = 1;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.stackPointer == 0x0);
  }

  SECTION("A program can exchange the DE and HL register pairs")
  {
    uint8_t program[1] = { XCHG };
    cpu.registerD = 0x33;
    cpu.registerE = 0x55;
    cpu.registerH = 0x00;
    cpu.registerL = 0xff;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerD == 0x00);
    REQUIRE(cpu.registerE == 0xff);
    REQUIRE(cpu.registerH == 0x33);
    REQUIRE(cpu.registerL == 0x55);
  }

  SECTION("A program can exchange the HL register pair with memory referenced by the stack pointer")
  {
    uint8_t program[1] = { XTHL };
    cpu.registerH = 0x0b;
    cpu.registerL = 0x3c;
    cpu.stackPointer = 0x10ad;
    cpu.memory[0x10ad] = 0x51;
    cpu.memory[0x10ae] = 0x67;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerH == 0x67);
    REQUIRE(cpu.registerL == 0x51);
    REQUIRE(cpu.memory[0x10ad] == 0x3c);
    REQUIRE(cpu.memory[0x10ae] == 0x0b);
  }

  SECTION("A program can replace the stack pointer with the HL register pair")
  {
    uint8_t program[1] = { SPHL };
    cpu.registerH = 0x33;
    cpu.registerL = 0x44;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.stackPointer == 0x3344);
  }
}


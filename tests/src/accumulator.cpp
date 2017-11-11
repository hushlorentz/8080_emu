#include "../../src/cpu.h"
#include "../../src/op_codes.h"

#include "catch.hpp"

using namespace Catch;

TEST_CASE("The CPU handles operations in the accumulator with operands from the registers and main memory")
{
  CPU cpu;

  SECTION("Register B can be added and stored in the accumulator")
  {
    uint8_t program[1] = { ADD_B };
    uint8_t initialValue = 0x0A;
    uint8_t value = 0x0B;
    cpu.registerA = initialValue;
    cpu.registerB = value;
    
    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == value + initialValue);
  }

  SECTION("Adding 0x6c and 0x2e does not set zero and carry, but does set sign, parity and auxiliary flags")
  {
    uint8_t program[1] = { ADD_B };
    uint8_t initialValue = 0x6c;
    uint8_t value = 0x2e;
    cpu.registerA = initialValue;
    cpu.registerB = value;
    
    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == value + initialValue);
    REQUIRE(!cpu.zeroBitSet());
    REQUIRE(!cpu.carryBitSet());
    REQUIRE(cpu.parityBitSet());
    REQUIRE(cpu.auxiliaryCarryBitSet());
    REQUIRE(cpu.signBitSet());
  }

  SECTION("Adding 0 and 0 sets the zero flag")
  {
    uint8_t program[1] = { ADD_B };
    uint8_t initialValue = 0x0;
    uint8_t value = 0x0;
    cpu.registerA = initialValue;
    cpu.registerB = value;
    
    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == value + initialValue);
    REQUIRE(cpu.zeroBitSet());
  }

  SECTION("Adding 10 and -10 sets the zero flag")
  {
    uint8_t program[1] = { ADD_B };
    uint8_t initialValue = 0xf6;
    uint8_t value = 0x0a;
    cpu.registerA = initialValue;
    cpu.registerB = value;
    
    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == (uint8_t)(value + initialValue));
    REQUIRE(cpu.zeroBitSet());
  }

  SECTION("Adding -10 and -10 sets the sign flag")
  {
    uint8_t program[1] = { ADD_B };
    uint8_t initialValue = 0xf6;
    uint8_t value = 0xf6;
    cpu.registerA = initialValue;
    cpu.registerB = value;
    
    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == (uint8_t)(value + initialValue));
    REQUIRE(cpu.signBitSet());
  }

  SECTION("Adding 0xFF and 0xFF sets the carry bit")
  {
    uint8_t program[1] = { ADD_B };
    uint8_t initialValue = 0xff;
    uint8_t value = 0xff;
    cpu.registerA = initialValue;
    cpu.registerB = value;
    
    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == (uint8_t)(initialValue + value));
    REQUIRE(cpu.carryBitSet());
    REQUIRE(cpu.auxiliaryCarryBitSet());
  }

  SECTION("Adding 0x6c and 0x2e from memory does not set zero and carry, but does set sign, parity and auxiliary flags")
  {
    uint8_t program[1] = { ADD_M };
    uint8_t initialValue = 0x6c;
    uint8_t value = 0x2e;
    cpu.registerA = initialValue;
    cpu.memory[0] = value;
    
    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == value + initialValue);
    REQUIRE(!cpu.zeroBitSet());
    REQUIRE(!cpu.carryBitSet());
    REQUIRE(cpu.parityBitSet());
    REQUIRE(cpu.auxiliaryCarryBitSet());
    REQUIRE(cpu.signBitSet());
  }

  SECTION("Adding all registers (including the accumulator) results in two times the sum of all registers in the accumulator")
  {
    uint8_t program[8] = { ADD_B, ADD_C, ADD_D, ADD_E, ADD_H, ADD_L, ADD_M, ADD_A };
    cpu.registerB = 0x01;
    cpu.registerC = 0x02;
    cpu.registerD = 0x03;
    cpu.registerE = 0x04;
    cpu.registerH = 0x05;
    cpu.registerL = 0x06;
    cpu.memory[0x0506] = 0x07;
    
    cpu.processProgram(program, 8);

    REQUIRE(cpu.registerA == 2 * (cpu.registerB + cpu.registerC + cpu.registerD + cpu.registerE + cpu.registerH + cpu.registerL + cpu.memory[0x0506]));
  }

  SECTION("If the carry bit is set, and the accumulator holds 0x42 and register D holds 0x3D, the accumulator will store 0x80 accounting for the carry.")
  {
    uint8_t program[2] = { STC, ADC_D };
    cpu.registerA = 0x42;
    cpu.registerD = 0x3d;

    cpu.processProgram(program, 2);

    REQUIRE(cpu.registerA == 0x80);
    REQUIRE(!cpu.zeroBitSet());
    REQUIRE(!cpu.carryBitSet());
    REQUIRE(!cpu.parityBitSet());
    REQUIRE(cpu.auxiliaryCarryBitSet());
    REQUIRE(cpu.signBitSet());
  }

  SECTION("If the carry bit is not set, and the accumulator holds 0x42 and register D holds 0x3D, the accumulator will store 0x7F accounting for the carry.")
  {
    uint8_t program[1] = { ADC_D };
    cpu.registerA = 0x42;
    cpu.registerD = 0x3d;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == 0x7f);
  }

  SECTION("A program can subtract a register from the accumulator")
  {
    uint8_t program[1] = { SUB_H };
    cpu.registerA = 0x0d;
    cpu.registerH = 0x08;

    cpu.processProgram(program, 1);
    REQUIRE(cpu.registerA == 0x05);
  }

  SECTION("Subtracting 10 from 5 in the accumulator sets the sign flag")
  {
    uint8_t program[1] = { SUB_B };
    cpu.registerA = 0x05;
    cpu.registerB = 0x0a;

    cpu.processProgram(program, 1);
    REQUIRE(cpu.registerA == (uint8_t)-5);
    REQUIRE(cpu.signBitSet());
  }

  SECTION("If subtracting from the accumulator does not cause a carry, the carry flag is set")
  {
    uint8_t program[1] = { SUB_D };
    cpu.registerA = 0x05;
    cpu.registerD = 0x64;

    cpu.processProgram(program, 1);
    REQUIRE(cpu.registerA == (uint8_t)(0x05 - 0x64));
    REQUIRE(cpu.carryBitSet());
  }

  SECTION("If subtracting from the accumulator does cause a carry, the carry flag is not set")
  {
    uint8_t program[1] = { SUB_L };
    cpu.registerA = 0x3e;
    cpu.registerL = 0x3e;

    cpu.processProgram(program, 1);
    REQUIRE(cpu.registerA == 0);
    REQUIRE(!cpu.carryBitSet());
  }

  SECTION("Subtractions from the accumulator cause the zero flag to be set if the result is 0")
  {
    uint8_t program[1] = { SUB_C };
    cpu.registerA = 0x06;
    cpu.registerC = 0x06;

    cpu.processProgram(program, 1);
    REQUIRE(cpu.registerA == 0);
    REQUIRE(cpu.zeroBitSet());
  }

  SECTION("A program can subtract a value in main memory from the accumulator")
  {
    uint8_t program[1] = { SUB_M };
    cpu.registerA = 0xcc;
    cpu.memory[0] = 0xbb;

    cpu.processProgram(program, 1);
    REQUIRE(cpu.registerA == 0x11);
  }

  SECTION("Subtracting the accumulator from itself always produces 0 and resets the carry bit")
  {
    uint8_t program[1] = { SUB_A };
    cpu.registerA = 0xcc;

    cpu.processProgram(program, 1);
    REQUIRE(cpu.registerA == 0);
    REQUIRE(cpu.zeroBitSet());
    REQUIRE(!cpu.carryBitSet());
  }

  SECTION("A program can subtract all the registers and main memory from the accumulator")
  {
    uint8_t program[7] = { SUB_B, SUB_C, SUB_D, SUB_E, SUB_H, SUB_L, SUB_M };
    cpu.registerA = 0xff;
    cpu.registerB = 0x1;
    cpu.registerC = 0x2;
    cpu.registerD = 0x3;
    cpu.registerE = 0x4;
    cpu.registerH = 0x5;
    cpu.registerL = 0x6;
    cpu.memory[0x0506] = 0x7;
    uint8_t expectedResult = 0xff - (0x1 + 0x2 + 0x3 + 0x4 + 0x5 + 0x6 + 0x7);

    cpu.processProgram(program, 7);
    REQUIRE(cpu.registerA == expectedResult);
  }

  SECTION("A program can perform a subtraction with a borrow")
  {
    uint8_t program[1] = { SBB_L };
    cpu.registerA = 0x04;
    cpu.registerL = 0x02;

    cpu.processProgram(program, 1);
    REQUIRE(cpu.registerA == 0x01);
    REQUIRE(!cpu.zeroBitSet());
    REQUIRE(!cpu.carryBitSet());
    REQUIRE(cpu.auxiliaryCarryBitSet());
    REQUIRE(!cpu.parityBitSet());
    REQUIRE(!cpu.signBitSet());
  }

  SECTION("A program can perform a logical AND with a register and the accumulator")
  {
    uint8_t program[1] = { ANA_D };
    cpu.registerA = 0x66;
    cpu.registerD = 0x06;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == 0x06);
  }

  SECTION("ANA ops set the correct status bits")
  {
    uint8_t program[2] = { STC, ANA_M };
    cpu.registerA = 0xF6;
    cpu.memory[0] = 0xF0;

    cpu.processProgram(program, 2);

    REQUIRE(cpu.registerA == 0xF0);
    REQUIRE(!cpu.carryBitSet());
    REQUIRE(!cpu.zeroBitSet());
    REQUIRE(cpu.parityBitSet());
    REQUIRE(cpu.signBitSet());
  }

  SECTION("A program can perform a logical XOR with a register and the accumulator")
  {
    uint8_t program[1] = { XRA_E };
    cpu.registerA = 0x66;
    cpu.registerE = 0x66;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == 0x0);
  }

  SECTION("XRA ops set the correct status bits")
  {
    uint8_t program[2] = { STC, XRA_M };
    cpu.registerA = 0x06;
    cpu.memory[0] = 0x60;

    cpu.processProgram(program, 2);

    REQUIRE(cpu.registerA == 0x66);
    REQUIRE(!cpu.carryBitSet());
    REQUIRE(!cpu.zeroBitSet());
    REQUIRE(cpu.parityBitSet());
    REQUIRE(!cpu.signBitSet());
  }

  SECTION("A program can perform a logical OR with a register and the accumulator")
  {
    uint8_t program[1] = { ORA_L };
    cpu.registerA = 0xab;
    cpu.registerL = 0x11;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == 0xbb);
  }

  SECTION("ORA ops set the correct status bits")
  {
    uint8_t program[2] = { STC, ORA_M };
    cpu.registerA = 0x33;
    cpu.memory[0] = 0x0F;

    cpu.processProgram(program, 2);

    REQUIRE(cpu.registerA == 0x3F);
    REQUIRE(!cpu.carryBitSet());
    REQUIRE(!cpu.zeroBitSet());
    REQUIRE(cpu.parityBitSet());
    REQUIRE(!cpu.signBitSet());
  }

  SECTION("The CMP_H opcode sets the zero bit if the contents of H equal the accumulator")
  {
    uint8_t program[1] = { CMP_H };
    cpu.registerA = 0xb5;
    cpu.registerH = 0xb5;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.zeroBitSet());
    REQUIRE(!cpu.carryBitSet());
  }

  SECTION("The CMP_B opcode sets the carry bit if register b is greater than the accumulator")
  {
    uint8_t program[1] = { CMP_B };
    cpu.registerA = -0x1b;
    cpu.registerB = -0x05;

    cpu.processProgram(program, 1);

    REQUIRE(!cpu.zeroBitSet());
    REQUIRE(cpu.carryBitSet());
  }

  SECTION("The CMP_M opcode sets neither the carry bit nor the zero bit if the value in memory is less than the accumulator")
  {
    uint8_t program[1] = { CMP_M };
    cpu.registerA = 0xff;
    cpu.memory[0] = 0xfe;

    cpu.processProgram(program, 1);
    
    REQUIRE(!cpu.zeroBitSet());
    REQUIRE(!cpu.carryBitSet());
  }
}

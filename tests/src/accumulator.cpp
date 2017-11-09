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
}

#include "../../src/cpu.h"
#include "../../src/op_codes.h"
#include "../../src/unhandled_op_code_exception.h"

#include "catch.hpp"

using namespace Catch;

TEST_CASE("The CPU handles opcodes for operations with immediate instructions")
{
  CPU cpu = CPU();
   
  SECTION("A program can load a register pair with immediate data")
  {
    uint8_t program[3] = { LXI_B, 0xff, 0xaa };

    cpu.loadProgram(program, 3);
    cpu.processProgram();

    REQUIRE(cpu.registerB == 0xaa);
    REQUIRE(cpu.registerC == 0xff);
  }

  SECTION("A program can replace the stack pointer with immediate data")
  {
    uint8_t program[3] = { LXI_SP, 0x01, 0x10 };

    cpu.loadProgram(program, 3);
    cpu.processProgram();

    REQUIRE(cpu.stackPointer == 0x1001);
  }

  SECTION("A program can replace the value of a register with immediate data")
  {
    uint8_t program[2] = { MVI_E, 0xa5 };

    cpu.loadProgram(program, 2);
    cpu.processProgram();

    REQUIRE(cpu.registerE == 0xa5);
  }

  SECTION("A program can replace a value in memory with immediate data")
  {
    uint8_t program[2] = { MVI_M, 0x6c };
    cpu.registerH = 0x77;
    cpu.registerL = 0x19;

    cpu.loadProgram(program, 2);
    cpu.processProgram();

    REQUIRE(cpu.memory[0x7719] == 0x6c);
  }

  SECTION("A program can add immediate data to the accumulator")
  {
    uint8_t program[2] = { ADI, 0x73 };
    cpu.registerA = 0x22;

    cpu.loadProgram(program, 2);
    cpu.processProgram();
    
    REQUIRE(cpu.registerA == 0x95);
  }

  SECTION("A program can add immediate data to the accumulator considering the carry")
  {
    uint8_t program[3] = { STC, ACI, 0x73 };
    cpu.registerA = 0x22;

    cpu.loadProgram(program, 3);
    cpu.processProgram();
    
    REQUIRE(cpu.registerA == 0x96);
  }

  SECTION("A program can subtract immediate data from the accumulator")
  {
    uint8_t program[2] = { SUI, 0x44 };
    cpu.registerA = 0x55;

    cpu.loadProgram(program, 2);
    cpu.processProgram();
    
    REQUIRE(cpu.registerA == 0x11);
  }

  SECTION("A program can subtract immediate data from the accumulator considering carry")
  {
    uint8_t program[3] = { STC, SBI, 0x01 };
    cpu.registerA = 0x00;

    cpu.loadProgram(program, 3);
    cpu.processProgram();
    
    REQUIRE(cpu.registerA == 0xfe);
    REQUIRE(cpu.carryBitSet());
    REQUIRE(cpu.signBitSet());
    REQUIRE(!cpu.zeroBitSet());
    REQUIRE(!cpu.parityBitSet());
    REQUIRE(!cpu.auxiliaryCarryBitSet());
  }

  SECTION("A program can logically AND the immediate data with the accumulator")
  {
    uint8_t program[2] = { ANI, 0x0f };
    cpu.registerA = 0x3a;

    cpu.loadProgram(program, 2);
    cpu.processProgram();

    REQUIRE(cpu.registerA == 0x0a);
  }

  SECTION("A program can logically XOR the immediate data with the accumulator")
  {
    uint8_t program[2] = { XRI, 0x81 };
    cpu.registerA = 0x3b;

    cpu.loadProgram(program, 2);
    cpu.processProgram();

    REQUIRE(cpu.registerA == 0xba);
  }

  SECTION("A program can logically OR the immediate data with the accumulator")
  {
    uint8_t program[2] = { ORI, 0x0f };
    cpu.registerA = 0xb5;

    cpu.loadProgram(program, 2);
    cpu.processProgram();

    REQUIRE(cpu.registerA == 0xbf);
  }

  SECTION("A program sets neither the zero bit nor the carry bit if the immediate data is less than the accumulator")
  {
    uint8_t program[2] = { CPI, 0x40 };
    cpu.registerA = 0x4a;

    cpu.loadProgram(program, 2);
    cpu.processProgram();

    REQUIRE(!cpu.zeroBitSet());
    REQUIRE(!cpu.carryBitSet());
  }

  SECTION("A program sets the zero bit if the immediate data is equal to the accumulator")
  {
    uint8_t program[2] = { CPI, 0x4a };
    cpu.registerA = 0x4a;

    cpu.loadProgram(program, 2);
    cpu.processProgram();

    REQUIRE(cpu.zeroBitSet());
    REQUIRE(!cpu.carryBitSet());
  }

  SECTION("A program sets the carry bit if the immediate data is greater than the accumulator")
  {
    uint8_t program[2] = { CPI, 0x55 };
    cpu.registerA = 0x4a;

    cpu.loadProgram(program, 2);
    cpu.processProgram();

    REQUIRE(!cpu.zeroBitSet());
    REQUIRE(cpu.carryBitSet());
  }
}

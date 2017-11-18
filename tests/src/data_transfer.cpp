#include "../../src/cpu.h"
#include "../../src/op_codes.h"

#include "catch.hpp"

using namespace Catch;

bool checkRegisterTransfer(CPU *cpu, uint8_t *program, uint8_t *src, uint8_t *dst)
{
  uint8_t value = 0x20;
  *src = value;
  cpu->loadProgram(program, 1);
  cpu->processProgram();
  return *dst == value;
}

bool checkRegisterTransferToM(CPU *cpu, uint8_t *program, uint8_t *src)
{
  uint8_t value = 0x20;
  *src = value;
  cpu->loadProgram(program, 1);
  cpu->processProgram();
  return cpu->registerM() == value;
}

bool checkRegisterTransferFromM(CPU *cpu, uint8_t *program, uint8_t *dst)
{
  uint8_t value = 0x20;
  cpu->registerH = 0xaa;
  cpu->registerL = 0xaa;
  cpu->memory[0xaaaa] = value;
  cpu->loadProgram(program, 1);
  cpu->processProgram();
  return *dst == value;
}

TEST_CASE("The CPU handles data transfer correctly") 
{
  CPU cpu = CPU();

  SECTION("A program can transfer a register to itself")
  {
    uint8_t program[1] = { MOV_B_B };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerB, &cpu.registerB));
  }

  SECTION("A program can transfer register C to register B")
  {
    uint8_t program[1] = { MOV_B_C };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerC, &cpu.registerB));
  }

  SECTION("A program can transfer register D to register B")
  {
    uint8_t program[1] = { MOV_B_D };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerD, &cpu.registerB));
  }

  SECTION("A program can transfer register E to register B")
  {
    uint8_t program[1] = { MOV_B_E };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerE, &cpu.registerB));
  }

  SECTION("A program can transfer register H to register B")
  {
    uint8_t program[1] = { MOV_B_H };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerH, &cpu.registerB));
  }

  SECTION("A program can transfer register L to register B")
  {
    uint8_t program[1] = { MOV_B_L };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerL, &cpu.registerB));
  }

  SECTION("A program can transfer register M to register B")
  {
    uint8_t program[1] = { MOV_B_M };
    REQUIRE(checkRegisterTransferFromM(&cpu, program, &cpu.registerB));
  }

  SECTION("A program can transfer register A to register B")
  {
    uint8_t program[1] = { MOV_B_A };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerA, &cpu.registerB));
  }

  SECTION("A program can transfer register B to register C")
  {
    uint8_t program[1] = { MOV_C_B };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerB, &cpu.registerC));
  }

  SECTION("A program can transfer register B to register D")
  {
    uint8_t program[1] = { MOV_D_B };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerB, &cpu.registerD));
  }

  SECTION("A program can transfer register B to register E")
  {
    uint8_t program[1] = { MOV_E_B };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerB, &cpu.registerE));
  }

  SECTION("A program can transfer register B to register H")
  {
    uint8_t program[1] = { MOV_H_B };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerB, &cpu.registerH));
  }

  SECTION("A program can transfer register B to register L")
  {
    uint8_t program[1] = { MOV_L_B };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerB, &cpu.registerL));
  }

  SECTION("A program can transfer register B to register A")
  {
    uint8_t program[1] = { MOV_A_B };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerB, &cpu.registerA));
  }

  SECTION("A program can transfer register B to register M")
  {
    uint8_t program[1] = { MOV_M_B };
    REQUIRE(checkRegisterTransferToM(&cpu, program, &cpu.registerB));
  }
 
  SECTION("A program can store the memory location addressed by registers BC in the accumulator")
  {
    uint8_t program[1] = { LDX_B };
    uint8_t value = 0xAF;
    cpu.registerB = 0x0A;
    cpu.registerC = 0xF0;
    cpu.memory[0x0AF0] = value;

    cpu.loadProgram(program, 1);
    cpu.processProgram();

    REQUIRE(cpu.registerA == value);
  }

  SECTION("A program can store the memory location addressed by registers DE in the accumulator")
  {
    uint8_t program[1] = { LDX_D };
    uint8_t value = 0xAF;
    cpu.registerD = 0xA0;
    cpu.registerE = 0x0F;
    cpu.memory[0xA00F] = value;

    cpu.loadProgram(program, 1);
    cpu.processProgram();

    REQUIRE(cpu.registerA == value);
  }

  SECTION("A program can store the accumulator in the memory location addressed by registers BC")
  {
    uint8_t program[1] = { STAX_B };
    uint8_t value = 0xAF;
    cpu.registerB = 0x0C;
    cpu.registerC = 0x55;
    cpu.registerA = value;

    cpu.loadProgram(program, 1);
    cpu.processProgram();

    REQUIRE(cpu.memory[0x0C55] == value);
  }

  SECTION("A program can store the accumulator in the memory location addressed by registers DE")
  {
    uint8_t program[1] = { STAX_D };
    uint8_t value = 0xAF;
    cpu.registerD = 0xB1;
    cpu.registerE = 0x49;
    cpu.registerA = value;

    cpu.loadProgram(program, 1);
    cpu.processProgram();

    REQUIRE(cpu.memory[0xB149] == value);
  }
}


#include <iostream>

#include "../../src/cpu.h"
#include "../../src/op_codes.h"

#include "catch.hpp"

using namespace Catch;

bool checkRegisterTransfer(CPU *cpu, uint8_t *program, uint8_t *src, uint8_t *dst)
{
  uint8_t value = 0x20;
  *src = value;
  cpu->processProgram(program, 1);
  return *dst == value;
}

bool checkRegisterTransferToM(CPU *cpu, uint8_t *program, uint8_t *src)
{
  uint8_t value = 0x20;
  *src = value;
  cpu->processProgram(program, 1);
  return cpu->registerM() == value;
}

bool checkRegisterTransferFromM(CPU *cpu, uint8_t *program, uint8_t *dst)
{
  uint8_t value = 0x20;
  cpu->memory[0] = value;
  cpu->processProgram(program, 1);
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

  SECTION("A program can transfer register B to register C")
  {
    uint8_t program[1] = { MOV_C_B };
    REQUIRE(checkRegisterTransfer(&cpu, program, &cpu.registerB, &cpu.registerC));
  }

  SECTION("A program can transfer register B to register M")
  {
    uint8_t program[1] = { MOV_M_B };
    REQUIRE(checkRegisterTransferToM(&cpu, program, &cpu.registerB));
  }
 
  SECTION("A program can transfer register M to itself")
  {
    uint8_t program[1] = { MOV_M_M };
    uint8_t value = 0x55;
    cpu.memory[0xAA] = value;
    cpu.registerL = 0xAA;

    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerM() == value);
  }
}


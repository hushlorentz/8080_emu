#include "../../src/cpu.h"
#include "../../src/op_codes.h"

#include "catch.hpp"

using namespace Catch;

TEST_CASE("The CPU handles data transfer correctly") 
{
  CPU cpu = CPU();

  SECTION("A program can transfer register B to register C")
  {
    uint8_t program[1] = { MOV_B_C };
    cpu.registerB = 0x20;

    REQUIRE(cpu.registerC == 0);
    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerB == cpu.registerC);
  }

  SECTION("A program can transfer register M to register A")
  {
    uint8_t program[1] = { MOV_M_A };
    cpu.memory[0xAA] = 0xBB;
    cpu.registerL = 0xAA;

    REQUIRE(cpu.registerA == 0);
    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerA == cpu.registerM());
  }
 
  SECTION("A program can transfer register E to register M")
  {
    uint8_t program[1] = { MOV_E_M };
    cpu.registerL = 0x55;

    REQUIRE(cpu.registerM() == 0);
    cpu.processProgram(program, 1);

    REQUIRE(cpu.registerM() == cpu.registerE);
  }
}


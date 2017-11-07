#include "../../src/cpu.h"
#include "../../src/op_codes.h"
#include "../../src/unhandled_op_code_exception.h"

#include "catch.hpp"

using namespace Catch;

TEST_CASE("The CPU handles all the OpCodes correctly") 
{
  CPU cpu = CPU();
   
  SECTION("The CPU starts in a cleared state")
  {
    REQUIRE(cpu.allClear());
  }

  SECTION("An unhandled OpCode throws an exception")
  {
    uint8_t program[1] = {0xFF};

    REQUIRE_THROWS_WITH(cpu.processProgram(program, 1), Contains("Unhandled Op Code: 0xff"));
  }

  SECTION("A program with only NOP, doesn't change the state of the CPU")
  {
    uint8_t program[1] = {NOP};

    cpu.processProgram(program, 1);

    REQUIRE(cpu.allClear());
  }

  SECTION("A program can set the Carry Bit")
  {
    uint8_t program[1] = {STC};

    cpu.processProgram(program, 1);

    REQUIRE(cpu.carryBitSet());
  }

  SECTION("A program can flip the Carry Bit")
  {
    uint8_t program[1] = {CMC};

    cpu.processProgram(program, 1);
    REQUIRE(cpu.carryBitSet());
  }

  SECTION("A program can reset the Carry Bit by flipping it twice")
  {
    uint8_t program[2] = {CMC, CMC};

    cpu.processProgram(program, 2);
    REQUIRE(!cpu.carryBitSet());
  }

  SECTION("A program can increment the registers")
  {
    uint8_t program[8] = {INR_A, INR_B, INR_C, INR_D, INR_E, INR_H, INR_L, INR_M};

    cpu.processProgram(program, 8);
    REQUIRE(cpu.registerA == 1);
    REQUIRE(cpu.registerB == 1);
    REQUIRE(cpu.registerC == 1);
    REQUIRE(cpu.registerD == 1);
    REQUIRE(cpu.registerE == 1);
    REQUIRE(cpu.registerH == 1);
    REQUIRE(cpu.registerL == 1);
    REQUIRE(cpu.registerM() == 1);
  }

  SECTION("Incrementing an 8-bit register once does not set the parity flag")
  {
    uint8_t program[1] = {INR_B};

    cpu.processProgram(program, 1);
    REQUIRE(!cpu.parityBitSet());
  }

  SECTION("Incrementing an 8-bit register three times sets the parity flag")
  {
    uint8_t program[3] = {INR_B, INR_B, INR_B};

    cpu.processProgram(program, 3);
    REQUIRE(cpu.parityBitSet());
  }

  SECTION("Incrementing a 16-bit register once does not set the parity flag")
  {
    uint8_t program[1] = {INR_M};

    cpu.processProgram(program, 1);
    REQUIRE(!cpu.parityBitSet());
  }

  SECTION("Incrementing a 16-bit register three times sets the parity flag")
  {
    uint8_t program[3] = {INR_M, INR_M, INR_M};

    cpu.processProgram(program, 3);
    REQUIRE(cpu.parityBitSet());
  }

  SECTION("A program can decrement all registers")
  {
    uint8_t program[8] = {DCR_A, DCR_B, DCR_C, DCR_D, DCR_E, DCR_H, DCR_L, DCR_M};

    cpu.processProgram(program, 8);
    REQUIRE((int8_t)cpu.registerA == -1);
    REQUIRE((int8_t)cpu.registerB == -1);
    REQUIRE((int8_t)cpu.registerC == -1);
    REQUIRE((int8_t)cpu.registerD == -1);
    REQUIRE((int8_t)cpu.registerE == -1);
    REQUIRE((int8_t)cpu.registerH == -1);
    REQUIRE((int8_t)cpu.registerL == -1);
    REQUIRE((int8_t)cpu.registerM() == -1);
  }

  SECTION("Decrementing a register and then incrementing it sets the zero flag")
  {
    uint8_t program[2] = {DCR_B, INR_B};

    cpu.processProgram(program, 2);
    REQUIRE(cpu.zeroBitSet());
  }

  SECTION("Incrementing register M and then decrementing it sets the zero flag")
  {
    uint8_t program[2] = {INR_M, DCR_M};

    cpu.processProgram(program, 2);
    REQUIRE(cpu.zeroBitSet());
  }

  SECTION("Decrementing a register passed 0 sets the sign flag")
  {
    uint8_t program[1] = {DCR_B};

    cpu.processProgram(program, 1);
    REQUIRE(cpu.signBitSet());
  }

  SECTION("Decrementing a register passed 0 and then incrementing resets the sign flag")
  {
    uint8_t program[2] = {DCR_B, INR_B};

    cpu.processProgram(program, 2);
    REQUIRE(!cpu.signBitSet());
  }

  SECTION("Incrementing a register containing 14 does not set the auxiliary carry flag")
  {
    cpu.registerD = 14;

    uint8_t program[1] = {INR_D};

    cpu.processProgram(program, 1);
    REQUIRE(!cpu.auxiliaryCarryBitSet());
  }

  SECTION("Incrementing a register containing 15 sets the auxiliary carry flag")
  {
    cpu.registerD = 15;

    uint8_t program[1] = {INR_D};

    cpu.processProgram(program, 1);
    REQUIRE(cpu.auxiliaryCarryBitSet());
  }

  SECTION("Incrementing register M containing 17 does not set the auxiliary carry flag")
  {
    cpu.memory[0] = 17;

    uint8_t program[1] = {INR_M};

    cpu.processProgram(program, 1);
    REQUIRE(!cpu.auxiliaryCarryBitSet());
  }

  SECTION("Incrementing register M containing 15 sets the auxiliary carry flag")
  {
    cpu.memory[0] = 15;

    uint8_t program[1] = {INR_M};

    cpu.processProgram(program, 1);
    REQUIRE(cpu.auxiliaryCarryBitSet());
  }

  SECTION("A program can set the accumulator to its compliment")
  {
    uint8_t program[1] = {CMA};

    cpu.processProgram(program, 1);
    REQUIRE((int8_t)cpu.registerA == -1);
  }

  SECTION("If the accumulator holds 0, DAA will not change its value")
  {
    uint8_t program[1] = {DAA};
    cpu.registerA = 0;

    cpu.processProgram(program, 1);
    REQUIRE(cpu.registerA == 0);
  }

  SECTION("If the accumulator holds 0xA, it will hold 0x10 after a DAA op code and the auxiliary flag will be set")
  {
    uint8_t program[1] = {DAA};
    cpu.registerA = 10;
    REQUIRE(cpu.registerA == 10);

    cpu.processProgram(program, 1);
    REQUIRE(cpu.registerA == 16);
    REQUIRE(cpu.auxiliaryCarryBitSet());
    REQUIRE(!cpu.carryBitSet());
  }

  SECTION("If the accumulator holds 0x9A and a program calls the DAA Op Code, the accumulator will hold 1 and the Auxiliary Carry and Carry flags will be set.")
  {
    uint8_t program[1] = {DAA};
    cpu.registerA = 0x9B;

    cpu.processProgram(program, 1);
    REQUIRE(cpu.registerA == 1);
    REQUIRE(cpu.carryBitSet());
    REQUIRE(cpu.auxiliaryCarryBitSet());
  }

  SECTION("If the accumulator holds 0, and the program sets the carry bit and does a DAA, the accumulator will hold 0xA0 and the carry flag will be unset.")
  {
    uint8_t program[2] = {STC, DAA};

    cpu.processProgram(program, 2);
    REQUIRE(cpu.registerA == 0x60);
    REQUIRE(!cpu.carryBitSet());
  }

  SECTION("If the accumulator holds 0XF and a program does a INR_A and DAA, the accumulator will hold 0x06 and the auxiliary carry flag will be unset.")
  {
    uint8_t program[2] = {INR_A, DAA};
    cpu.registerA = 0xF;

    cpu.processProgram(program, 2);
    REQUIRE(cpu.registerA == 0x16);
    REQUIRE(!cpu.auxiliaryCarryBitSet());
  }
}

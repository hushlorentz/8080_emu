#include <iostream>
#include <string>

#include "bit_ops.h"
#include "cpu.h"
#include "op_codes.h"
#include "status_bits.h"
#include "unhandled_op_code_exception.h"

using namespace std;

CPU::CPU() : registerA(0), registerB(0), registerC(0), registerD(0), registerE(0), registerH(0), registerL(0), status(0)
{
  memory.resize(MAX_MEMORY);
}

bool CPU::carryBitSet()
{
  return hasFlag(status, CARRY_BIT);
}

void CPU::processProgram(uint8_t *program, uint16_t programSize)
{
  for (uint8_t *pc = program; pc < program + programSize; pc++)
  {
    switch (*pc)
    {
      case NOP:
        break;
      case INR_B:
        addToRegister(&(registerB), 1);
        break;  
      case INR_C:
        addToRegister(&(registerC), 1);
        break;  
      case INR_D:
        addToRegister(&(registerD), 1);
        break;  
      case INR_E:
        addToRegister(&(registerE), 1);
        break;  
      case INR_H:
        addToRegister(&(registerH), 1);
        break;  
      case INR_L:
        addToRegister(&(registerL), 1);
        break;  
      case INR_A:
        addToRegister(&(registerA), 1);
        break;  
      case INR_M:
        addToRegisterM(1);
        break;  
      case STC:
        setStatus(CARRY_BIT);
        break;
      case CMC:
        flipStatusBit(CARRY_BIT);
        break;
      default:
        throw UnhandledOpCodeException();
        break;  
    }
  }
}

void CPU::setStatus(uint8_t bit)
{
  setFlag(&(status), bit);
}

void CPU::clearStatus(uint8_t bit)
{
  clearFlag(&(status), bit);
}

void CPU::flipStatusBit(uint8_t bit)
{
  status = (status & bit) ? status & ~bit : status |= bit;
}

bool CPU::allClear()
{
  return status == 0 && registerB == 0 && registerC == 0 &&
    registerD == 0 && registerE == 0 && registerH == 0 &&
    registerL == 0 && registerA == 0;
}

void CPU::addToRegister(uint8_t *reg, uint8_t operand)
{
  *reg += operand;
  setParityBitFrom8BitRegister(*reg);
}

void CPU::setParityBitFrom8BitRegister(uint8_t reg)
{
  reg ^= reg >> 4;
  reg &= 0xf;
  ((0x9669 >> reg) & 1) ? setStatus(PARITY_BIT) : clearStatus(PARITY_BIT);
}

void CPU::setParityBitFrom16BitRegister(uint16_t reg)
{
  setParityBitFrom8BitRegister(reg ^ reg >> 8);
}

void CPU::addToRegisterM(uint16_t operand)
{
  uint16_t memory_address = currentMemoryAddress();
  uint8_t sum = memory[memory_address] + operand;

  memory[memory_address] = sum;
  setParityBitFrom16BitRegister(registerM());
}

uint8_t CPU::registerM()
{
  return memory[currentMemoryAddress()];
}

uint16_t CPU::currentMemoryAddress()
{
  return registerH << 8 | registerL;
}

bool CPU::parityBitSet()
{
  return hasFlag(status, PARITY_BIT);
}

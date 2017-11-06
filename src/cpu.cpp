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
        incrementRegister(&registerB);
        break;  
      case INR_C:
        incrementRegister(&registerC);
        break;  
      case INR_D:
        incrementRegister(&registerD);
        break;  
      case INR_E:
        incrementRegister(&registerE);
        break;  
      case INR_H:
        incrementRegister(&registerH);
        break;  
      case INR_L:
        incrementRegister(&registerL);
        break;  
      case INR_A:
        incrementRegister(&registerA);
        break;  
      case INR_M:
        incrementRegisterM();
        break;  
      case STC:
        setStatus(CARRY_BIT);
        break;
      case DCR_B:
        decrementRegister(&registerB);
        break;
      case DCR_C:
        decrementRegister(&registerC);
        break;
      case DCR_D:
        decrementRegister(&registerD);
        break;
      case DCR_E:
        decrementRegister(&registerE);
        break;
      case DCR_H:
        decrementRegister(&registerH);
        break;
      case DCR_L:
        decrementRegister(&registerL);
        break;
      case DCR_A:
        decrementRegister(&registerA);
        break;
      case DCR_M:
        decrementRegisterM();
        break;
      case CMC:
        flipStatusBit(CARRY_BIT);
        break;
      case CMA:
        complimentAccumulator();
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

void CPU::incrementRegister(uint8_t *reg)
{
  (*reg)++; 
  setParityBitFrom8BitRegister(*reg);
}

void CPU::setParityBitFrom8BitRegister(uint8_t reg)
{
  reg ^= reg >> 4;
  reg &= 0xf;
  ((0x9669 >> reg) & 1) ? setStatus(PARITY_BIT) : clearStatus(PARITY_BIT);
}

void CPU::decrementRegister(uint8_t *reg)
{
  (*reg)--;
  setParityBitFrom8BitRegister(*reg);
}

void CPU::incrementRegisterM()
{
  uint16_t memory_address = currentMemoryAddress();
  uint8_t sum = memory[memory_address] + 1;

  memory[memory_address] = sum;
  setParityBitFrom16BitRegister(registerM());
}

void CPU::setParityBitFrom16BitRegister(uint16_t reg)
{
  setParityBitFrom8BitRegister(reg ^ reg >> 8);
}

void CPU::decrementRegisterM()
{
  uint16_t memory_address = currentMemoryAddress();
  uint8_t sum = memory[memory_address] - 1;

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

void CPU::complimentAccumulator()
{
  registerA = ~registerA;
}

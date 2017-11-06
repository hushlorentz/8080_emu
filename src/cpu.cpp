#include <iostream>

#include "bit_ops.h"
#include "cpu.h"
#include "op_codes.h"
#include "status_bits.h"
#include "unhandled_op_code_exception.h"

using namespace std;

CPU::CPU() : registerA(0), registerB(0), registerC(0), registerD(0), registerE(0), registerH(0), registerL(0), status(0)
{
}

bool CPU::carryBitSet()
{
  return hasFlag(this->status, CARRY_BIT);
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
        this->addToRegister(&(this->registerB), 1);
        break;  
      case INR_C:
        this->addToRegister(&(this->registerC), 1);
        break;  
      case INR_D:
        this->addToRegister(&(this->registerD), 1);
        break;  
      case INR_E:
        this->addToRegister(&(this->registerE), 1);
        break;  
      case INR_H:
        this->addToRegister(&(this->registerH), 1);
        break;  
      case INR_L:
        this->addToRegister(&(this->registerL), 1);
        break;  
      case INR_A:
        this->addToRegister(&(this->registerA), 1);
        break;  
      case INR_M:
        this->addToRegisterM(1);
        break;  
      case STC:
        this->setStatus(CARRY_BIT);
        break;
      case CMC:
        this->flipStatusBit(CARRY_BIT);
        break;
      default:
        throw UnhandledOpCodeException();
        break;  
    }
  }
}

void CPU::setStatus(uint8_t bit)
{
  setFlag(&(this->status), bit);
}

void CPU::clearStatus(uint8_t bit)
{
  clearFlag(&(this->status), bit);
}

void CPU::flipStatusBit(uint8_t bit)
{
  this->status = (this->status & bit) ? this->status & ~bit : this->status |= bit;
}

bool CPU::allClear()
{
  return this->status == 0 && this->registerB == 0 && this->registerC == 0 &&
    this->registerD == 0 && this->registerE == 0 && this->registerH == 0 &&
    this->registerL == 0 && this->registerA == 0;
}

void CPU::addToRegister(uint8_t *reg, uint8_t operand)
{
  *reg += operand;
  this->setParityBitFrom8BitRegister(*reg);
}

void CPU::setParityBitFrom8BitRegister(uint8_t reg)
{
  reg ^= reg >> 4;
  reg &= 0xf;
  ((0x9669 >> reg) & 1) ? setStatus(PARITY_BIT) : clearStatus(PARITY_BIT);
}

void CPU::setParityBitFrom16BitRegister(uint16_t reg)
{
  this->setParityBitFrom8BitRegister(reg ^ reg >> 8);
}

void CPU::addToRegisterM(uint16_t operand)
{
  uint16_t HL = this->registerH << 8 | this->registerL;
  HL += operand;

  this->registerH = HL >> 8 & 0xFF;
  this->registerL = HL & 0xFF;
  this->setParityBitFrom16BitRegister(this->registerM());
}

uint16_t CPU::registerM()
{
  uint16_t regH = this->registerH;
  return regH << 8 | this->registerL;
}

bool CPU::parityBitSet()
{
  return hasFlag(this->status, PARITY_BIT);
}

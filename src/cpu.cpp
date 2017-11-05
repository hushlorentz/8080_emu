#include <iostream>

#include "cpu.h"
#include "op_codes.h"
#include "status_bits.h"
#include "unhandled_op_code_exception.h"

using namespace std;

CPU::CPU() : status(0)
{
}

char CPU::carryBit()
{
  return this->status & CARRY_BIT;
}

void CPU::processProgram(unsigned char *program, int programSize)
{
  for (unsigned char *pc = program; pc < program + programSize; pc++)
  {
    switch (*pc)
    {
      case NOP:
        break;
      case STC:
        this->status |= CARRY_BIT;
        break;
      case CMC:
        this->status = this->carryBit() ? this->status & ~CARRY_BIT : this->status |= CARRY_BIT;
        break;
      default:
        throw UnhandledOpCodeException();
        break;  
    }
  }
}

bool CPU::allClear()
{
  return this->status == 0;
}

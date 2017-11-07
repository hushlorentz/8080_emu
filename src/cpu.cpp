#include <iostream>
#include <string>

#include "bit_ops.h"
#include "cpu.h"
#include "op_codes.h"
#include "status_bits.h"
#include "unhandled_op_code_exception.h"

using namespace std;

#define REGISTER_A 7
#define REGISTER_B 0
#define REGISTER_C 1
#define REGISTER_D 2
#define REGISTER_E 3
#define REGISTER_H 4
#define REGISTER_L 5
#define REGISTER_M 6

CPU::CPU() : registerA(0), registerB(0), registerC(0), registerD(0), registerE(0), registerH(0), registerL(0), status(0)
{
  memory.resize(MAX_MEMORY);
  registerMap[0] = &registerB;
  registerMap[1] = &registerC;
  registerMap[2] = &registerD;
  registerMap[3] = &registerE;
  registerMap[4] = &registerH;
  registerMap[5] = &registerL;
  registerMap[7] = &registerA;
}

bool CPU::carryBitSet()
{
  return hasFlag(status, CARRY_BIT);
}

bool CPU::parityBitSet()
{
  return hasFlag(status, PARITY_BIT);
}

bool CPU::signBitSet()
{
  return hasFlag(status, SIGN_BIT);
}

bool CPU::zeroBitSet()
{
  return hasFlag(status, ZERO_BIT);
}

bool CPU::auxiliaryCarryBitSet()
{
  return hasFlag(status, AUXILIARY_CARRY_BIT);
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
      case DAA:
        decimalAdjustAccumulator();  
        break;
      case MOV_B_B:
      case MOV_B_C:
      case MOV_B_D:
      case MOV_B_E:
      case MOV_B_H:
      case MOV_B_L:
      case MOV_B_M:
      case MOV_B_A:
      case MOV_C_B:
      case MOV_D_B:
      case MOV_E_B:
      case MOV_H_B:
      case MOV_L_B:
      case MOV_A_B:
      case MOV_E_M:
      case MOV_M_B:
      case MOV_M_A:
      case MOV_M_M:
        moveRegisterToRegister(*pc);
        break;  
      default:
        throw UnhandledOpCodeException(*pc);
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
  setAuxiliaryCarryBitFromRegisterAndOperand(*reg, 1);

  (*reg)++; 
  setStatusFromRegister(*reg);
}

void CPU::setStatusFromRegister(uint8_t reg)
{
  setParityBitFromRegister(reg);
  setZeroBitFromRegister(reg);
  setSignBitFromRegister(reg);
}

void CPU::setParityBitFromRegister(uint8_t reg)
{
  reg ^= reg >> 4;
  reg &= 0xf;
  ((0x9669 >> reg) & 1) ? setStatus(PARITY_BIT) : clearStatus(PARITY_BIT);
}

void CPU::setZeroBitFromRegister(uint8_t reg)
{
  reg == 0 ? setStatus(ZERO_BIT) : clearStatus(ZERO_BIT);
}

void CPU::setSignBitFromRegister(uint8_t reg)
{
  (int8_t)reg < 0 ? setStatus(SIGN_BIT) : clearStatus(SIGN_BIT);
}

void CPU::setAuxiliaryCarryBitFromRegisterAndOperand(uint8_t reg, uint8_t operand)
{
  checkAuxiliaryCarryBitFromRegisterAndOperand(reg, operand) ? setStatus(AUXILIARY_CARRY_BIT) : clearStatus(AUXILIARY_CARRY_BIT);
}

bool CPU::checkAuxiliaryCarryBitFromRegisterAndOperand(uint8_t reg, uint8_t operand)
{
  uint8_t regAuxCarryBit = (reg >> AUXILIARY_CARRY_SHIFT) & 1;
  uint8_t sumAuxCarryBit = ((reg + operand) >> AUXILIARY_CARRY_SHIFT) & 1;

  return (regAuxCarryBit == 1 && sumAuxCarryBit == 0);
}

void CPU::decrementRegister(uint8_t *reg)
{
  (*reg)--;
  setStatusFromRegister(*reg);
}

void CPU::incrementRegisterM()
{
  uint16_t memory_address = currentMemoryAddress();
  uint8_t sum = memory[memory_address] + 1;

  setAuxiliaryCarryBitFromRegisterAndOperand(memory[memory_address], 1);

  memory[memory_address] = sum;
  setStatusFromRegister(registerM());
}

void CPU::decrementRegisterM()
{
  uint16_t memory_address = currentMemoryAddress();
  uint8_t sum = memory[memory_address] - 1;

  memory[memory_address] = sum;
  setStatusFromRegister(registerM());
}

uint8_t CPU::registerM()
{
  return memory[currentMemoryAddress()];
}

uint16_t CPU::currentMemoryAddress()
{
  return registerH << 8 | registerL;
}

void CPU::complimentAccumulator()
{
  registerA = ~registerA;
}

void CPU::decimalAdjustAccumulator()
{
  uint8_t lower_bits = getLowerNibble(registerA);
  uint8_t operand = 6;

  if (lower_bits > 9 || auxiliaryCarryBitSet())
  {
    setAuxiliaryCarryBitFromRegisterAndOperand(registerA, operand);
    registerA += operand;
  }

  uint8_t upper_bits = getUpperNibble(registerA);
  if (upper_bits > 9 || carryBitSet())
  {
    checkAuxiliaryCarryBitFromRegisterAndOperand(upper_bits, operand) ? setStatus(CARRY_BIT) : clearStatus(CARRY_BIT);
    upper_bits += operand;
  }

  registerA &= 0x0F;
  registerA |= upper_bits << 4;
}

void CPU::setCarryBitFromRegisterAndOperand(uint8_t reg, uint8_t operand)
{
  checkCarryBitFromRegisterAndOperand(reg, operand) ? setStatus(CARRY_BIT) : clearStatus(CARRY_BIT);
}

bool CPU::checkCarryBitFromRegisterAndOperand(uint8_t reg, uint8_t operand)
{
  uint8_t regCarryBit = (reg >> CARRY_SHIFT) & 1;
  uint8_t sumCarryBit = ((reg + operand) >> CARRY_SHIFT) & 1;

  return (regCarryBit == 1 && sumCarryBit == 0);
}

void CPU::moveRegisterToRegister(uint8_t opCode)
{
  uint8_t dst = opCode >> 3 & 7;
  uint8_t src = opCode & 7;

  if (dst == REGISTER_M)
  {
    if (src != REGISTER_M)
    {
      memory[currentMemoryAddress()] = *registerMap[src];
    }
  }
  else if (src == REGISTER_M)
  {
    *registerMap[dst] = memory[currentMemoryAddress()];
  }
  else
  {
    *registerMap[dst] = *registerMap[src];
  }
}

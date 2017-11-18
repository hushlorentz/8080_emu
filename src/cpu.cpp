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

#define REGISTER_PAIR_B 0
#define REGISTER_PAIR_D 1
#define REGISTER_PAIR_H 2
#define REGISTER_PAIR_A 3

#define MAX_MEMORY 65536
#define NO_INTERRUPT 0xff

CPU::CPU() : followJumps(true), runProgram(true), registerA(0), registerB(0), registerC(0), registerD(0), registerE(0), registerH(0), registerL(0),  stackPointer(MAX_MEMORY), status(0x02), programCounter(0), stepThrough(false), interruptToHandle(NO_INTERRUPT), programLength(0)
{
  memory.resize(MAX_MEMORY);

  registerMap[REGISTER_B] = &registerB;
  registerMap[REGISTER_C] = &registerC;
  registerMap[REGISTER_D] = &registerD;
  registerMap[REGISTER_E] = &registerE;
  registerMap[REGISTER_H] = &registerH;
  registerMap[REGISTER_L] = &registerL;
  registerMap[REGISTER_A] = &registerA;

  registerPairB.push_back(&registerB);
  registerPairB.push_back(&registerC);
  registerPairD.push_back(&registerD);
  registerPairD.push_back(&registerE);
  registerPairH.push_back(&registerH);
  registerPairH.push_back(&registerL);
  registerPairA.push_back(&registerA);
  registerPairA.push_back(&status);

  registerPairMap[REGISTER_PAIR_B] = &registerPairB;
  registerPairMap[REGISTER_PAIR_D] = &registerPairD;
  registerPairMap[REGISTER_PAIR_H] = &registerPairH;
  registerPairMap[REGISTER_PAIR_A] = &registerPairA;
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

void CPU::loadProgram(uint8_t *program, uint16_t programSize)
{
  programCounter = 0;
  programLength = programSize;
  memcpy(memory.data(), program, programSize);
}

void CPU::processProgram()
{
  do
  {
    handleNextInstruction();
  }
  while (programCounter < programLength && runProgram && !stepThrough);
}

void CPU::handleNextInstruction()
{
  switch (memory[programCounter])
  {
    case LXI_B:
    case LXI_D:
    case LXI_H:
    case LXI_SP:
    case STA:
    case LDA:
    case SHLD:
    case LXLD:
      handle3ByteOp(memory[programCounter], memory[programCounter + 1], memory[programCounter + 2]);
      programCounter += 3;
      break;  
    case MVI_B:
    case MVI_C:
    case MVI_D:
    case MVI_E:
    case MVI_H:
    case MVI_L:  
    case MVI_M:  
    case MVI_A:  
    case ADI:
    case ACI:
    case SUI:
    case SBI:
    case ANI:
    case XRI:
    case ORI:
    case CPI:
      handle2ByteOp(memory[programCounter], memory[programCounter + 1]);  
      programCounter += 2;
      break;
    case PCHL:
      programCounter = followJumps ? handleJumpByteOp() : programCounter + 1;
      break;
    case JC:
    case JNC:
    case JZ:
    case JNZ:
    case JMP:
    case JM:
    case JP:
    case JPE:
    case JPO:
      programCounter = followJumps ? handleJump3ByteOp(memory[programCounter], memory[programCounter + 1], memory[programCounter + 2]) : programCounter + 3;
      break;
    case CALL:
    case CC:
    case CNC:
    case CZ:
    case CNZ:
    case CM:
    case CP:
    case CPE:
    case CPO:
      programCounter = followJumps ? handleCall3ByteOp(memory[programCounter], memory[programCounter + 1], memory[programCounter + 2]) : programCounter + 3;
      break;
    case RET:
    case RC:
    case RNC:
    case RZ:
    case RNZ:
    case RM:
    case RP:
    case RPE:
    case RPO:
      programCounter = followJumps ? handleReturnOp(memory[programCounter]) : programCounter + 1;
      break;
    default:
      handleByteOp(memory[programCounter]);
      programCounter++;
      break;
  }
}

void CPU::handleByteOp(uint8_t opCode)
{
  switch (opCode)
  {
      case MOV_B_B:
      case MOV_C_C:
      case MOV_D_D:
      case MOV_E_E:
      case MOV_H_H:
      case MOV_L_L:
      case MOV_M_M:
      case MOV_A_A:
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
      case MOV_B_C:
      case MOV_B_D:
      case MOV_B_E:
      case MOV_B_H:
      case MOV_B_L:
      case MOV_B_M:
      case MOV_B_A:
      case MOV_C_B:
      case MOV_C_D:
      case MOV_C_E:
      case MOV_C_H:
      case MOV_C_L:
      case MOV_C_M:
      case MOV_C_A:
      case MOV_D_B:
      case MOV_D_C:
      case MOV_D_E:
      case MOV_D_H:
      case MOV_D_L:
      case MOV_D_M:
      case MOV_D_A:
      case MOV_E_B:
      case MOV_E_C:
      case MOV_E_D:
      case MOV_E_H:
      case MOV_E_L:
      case MOV_E_M:
      case MOV_E_A:
      case MOV_H_B:
      case MOV_H_C:
      case MOV_H_D:
      case MOV_H_E:
      case MOV_H_L:
      case MOV_H_M:
      case MOV_H_A:
      case MOV_L_B:
      case MOV_L_C:
      case MOV_L_D:
      case MOV_L_E:
      case MOV_L_H:
      case MOV_L_M:
      case MOV_L_A:
      case MOV_M_B:
      case MOV_M_C:
      case MOV_M_D:
      case MOV_M_E:
      case MOV_M_H:
      case MOV_M_L:
      case MOV_M_A:
      case MOV_A_B:
      case MOV_A_C:
      case MOV_A_D:
      case MOV_A_E:
      case MOV_A_H:
      case MOV_A_L:
      case MOV_A_M:
        moveRegisterToRegister(opCode);
        break;  
      case LDX_B:
        moveMemoryToAccumulator(registerB, registerC);  
        break;
      case LDX_D:
        moveMemoryToAccumulator(registerD, registerE);  
        break;
      case STAX_B:
        moveAccumulatorToMemory(registerB, registerC);  
        break;
      case STAX_D:
        moveAccumulatorToMemory(registerD, registerE);  
        break;
      case ADD_B:
      case ADD_C:
      case ADD_D:
      case ADD_E:
      case ADD_H:
      case ADD_L:
      case ADD_A:
        addValueToAccumulator(*registerMap[opCode & 7], 0);
        break;
      case ADD_M:
        addValueToAccumulator(registerM(), 0);
        break;
      case ADC_B:
      case ADC_C:
      case ADC_D:
      case ADC_E:
      case ADC_H:
      case ADC_L:
      case ADC_A:
        addValueToAccumulator(registerValueFromOpCode(opCode), carryBitSet() ? 1 : 0);
        break;
      case ADC_M:
        addValueToAccumulator(registerM(), carryBitSet() ? 1 : 0);
        break;
      case SUB_B:
      case SUB_C:
      case SUB_D:
      case SUB_E:
      case SUB_H:
      case SUB_L:
      case SUB_A:
        subtractValueFromAccumulator(registerValueFromOpCode(opCode));
        break;  
      case SUB_M:
        subtractValueFromAccumulator(registerM());
        break;
      case SBB_B:
      case SBB_C:
      case SBB_D:
      case SBB_E:
      case SBB_H:
      case SBB_L:
      case SBB_A:
        subtractValueFromAccumulator(registerValueFromOpCode(opCode) + (carryBitSet() ? 1 : 0));
        break;
      case SBB_M:
        subtractValueFromAccumulator(registerM() + (carryBitSet() ? 1 : 0));
        break;
      case ANA_B:
      case ANA_C:
      case ANA_D:
      case ANA_E:
      case ANA_H:
      case ANA_L:
      case ANA_A:
        logicalANDWithAccumulator(registerValueFromOpCode(opCode));  
        break;
      case ANA_M:
        logicalANDWithAccumulator(registerM());  
        break;
      case XRA_B:
      case XRA_C:
      case XRA_D:
      case XRA_E:
      case XRA_H:
      case XRA_L:
      case XRA_A:
        logicalXORWithAccumulator(registerValueFromOpCode(opCode));  
        break;
      case XRA_M:
        logicalXORWithAccumulator(registerM());
        break;
      case ORA_B:
      case ORA_C:
      case ORA_D:
      case ORA_E:
      case ORA_H:
      case ORA_L:
      case ORA_A:
        logicalORWithAccumulator(registerValueFromOpCode(opCode));  
        break;
      case ORA_M:
        logicalORWithAccumulator(registerM());
        break;
      case CMP_B:
      case CMP_C:
      case CMP_D:
      case CMP_E:
      case CMP_H:
      case CMP_L:
      case CMP_A:
        compareValueToAccumulator(registerValueFromOpCode(opCode));
        break;
      case CMP_M:  
        compareValueToAccumulator(registerM());
        break;
      case RLC:
        rotateAccumulatorLeft();
        break;  
      case RRC:
        rotateAccumulatorRight();
        break;
      case RAL:
        rotateAccumulatorLeftWithCarry();
        break;  
      case RAR:
        rotateAccumulatorRightWithCarry();
        break;  
      case PUSH_B:
      case PUSH_D:
      case PUSH_H:
      case PUSH_PSW:  
        pushRegisterPairOnStack(registerPairFromOpCode(opCode));
        break; 
      case POP_B:
      case POP_D:
      case POP_H:
        popStackToRegisterPair(registerPairFromOpCode(opCode));
        break;
      case POP_PSW:
        popStackToAccumulatorAndStatusPair();
        break;
      case DAD_B:
      case DAD_D:
      case DAD_H:
        addValueToRegisterPairH(valueOfRegisterPair(registerPairFromOpCode(opCode)));  
        break;
      case DAD_SP:
        addValueToRegisterPairH(stackPointer);
        break;
      case INX_B:
      case INX_D:
      case INX_H:
        incrementRegisterPair(registerPairFromOpCode(opCode));
        break;  
      case INX_SP:
        stackPointer++;
        break;  
      case DCX_B:  
      case DCX_D:  
      case DCX_H:  
        decrementRegisterPair(registerPairFromOpCode(opCode));
        break;
      case DCX_SP:
        stackPointer--;
        break;
      case XCHG:
        exchangeRegisterPairs(registerPairMap[REGISTER_PAIR_D], registerPairMap[REGISTER_PAIR_H]);  
        break;
      case XTHL:
        exchangeRegistersAndMemory();
        break;
      case SPHL:
        stackPointer = registerH << 8 | registerL;
        break;
      case QUIT:
        runProgram = false;
        break;
      default:
        throw UnhandledOpCodeException(opCode);
        break;  
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
  return status == 0x02 && stackPointer == MAX_MEMORY && registerB == 0 && registerC == 0 &&
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
  return hasCarryAtBitIndex(reg, operand, AUXILIARY_CARRY_SHIFT);
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
  return hasCarryAtBitIndex(reg, operand, CARRY_SHIFT);
}

void CPU::moveRegisterToRegister(uint8_t opCode)
{
  uint8_t dst = opCode >> 3 & 7;
  uint8_t src = opCode & 7;

  if (dst == REGISTER_M)
  {
      memory[currentMemoryAddress()] = *registerMap[src];
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

void CPU::moveMemoryToAccumulator(uint8_t upperBitsAddress, uint8_t lowerBitsAddress)
{
  registerA = memory[upperBitsAddress << 8 | lowerBitsAddress];
}

void CPU::moveAccumulatorToMemory(uint8_t upperBitsAddress, uint8_t lowerBitsAddress)
{
  memory[upperBitsAddress << 8 | lowerBitsAddress] = registerA;
}

void CPU::addValueToAccumulator(uint8_t value, uint8_t carry)
{
  setAuxiliaryCarryBitFromRegisterAndOperand(registerA, value);
  setCarryBitFromRegisterAndOperand(registerA, value);
  registerA += value;
  setStatusFromRegister(registerA);

  if (carry)
  {
    addValueToAccumulator(1, 0);
  }
}

void CPU::subtractValueFromAccumulator(uint8_t value)
{
  checkCarryBitFromRegisterAndOperand(registerA, ~value + 1) ? clearStatus(CARRY_BIT) : setStatus(CARRY_BIT);
  checkAuxiliaryCarryBitFromRegisterAndOperand(registerA, ~value + 1) ? setStatus(AUXILIARY_CARRY_BIT) : clearStatus(AUXILIARY_CARRY_BIT);
  registerA += (~value + 1);
  setStatusFromRegister(registerA);
}

uint8_t CPU::registerValueFromOpCode(uint8_t opCode)
{
  return *registerMap[opCode & 7];
}

void CPU::logicalANDWithAccumulator(uint8_t value)
{
  clearStatus(CARRY_BIT);
  registerA &= value;
  setStatusFromRegister(registerA);
}

void CPU::logicalXORWithAccumulator(uint8_t value)
{
  clearStatus(CARRY_BIT);
  registerA ^= value;
  setStatusFromRegister(registerA);
}

void CPU::logicalORWithAccumulator(uint8_t value)
{
  clearStatus(CARRY_BIT);
  registerA |= value;
  setStatusFromRegister(registerA);
}

void CPU::compareValueToAccumulator(uint8_t value)
{
  checkCarryBitFromRegisterAndOperand(registerA, ~value + 1) ? clearStatus(CARRY_BIT) : setStatus(CARRY_BIT);
  checkAuxiliaryCarryBitFromRegisterAndOperand(registerA, ~value + 1) ? setStatus(AUXILIARY_CARRY_BIT) : clearStatus(AUXILIARY_CARRY_BIT);
  uint8_t diff = registerA + (~value + 1);
  setStatusFromRegister(diff);
}

void CPU::rotateAccumulatorLeft()
{
  bool highBitSet = (registerA >> CARRY_SHIFT & 1) == 1;
  highBitSet ? setStatus(CARRY_BIT) : clearStatus(CARRY_BIT);
  registerA <<= 1;
  registerA |= highBitSet ? 1 : 0;
}

void CPU::rotateAccumulatorRight()
{
  bool lowBitSet = (registerA & 1) == 1;
  lowBitSet ? setStatus(CARRY_BIT) : clearStatus(CARRY_BIT);
  registerA >>= 1;
  registerA |= lowBitSet ? 1 << CARRY_SHIFT : 0;
}

void CPU::rotateAccumulatorLeftWithCarry()
{
  bool carrySet = carryBitSet();
  (registerA >> CARRY_SHIFT & 1) == 1 ? setStatus(CARRY_BIT) : clearStatus(CARRY_BIT);
  registerA <<= 1;
  registerA |= carrySet ? 1 : 0;
}

void CPU::rotateAccumulatorRightWithCarry()
{
  bool carrySet = carryBitSet();
  (registerA & 1) == 1 ? setStatus(CARRY_BIT) : clearStatus(CARRY_BIT);
  registerA >>= 1;
  registerA |= carrySet ? 1 << CARRY_SHIFT : 0;
}

vector<uint8_t *> * CPU::registerPairFromOpCode(uint8_t opCode)
{
  return registerPairMap[(opCode >> 4) & 0x3];
}

void CPU::pushRegisterPairOnStack(vector<uint8_t *> * pair)
{
  memory[stackPointer - 1] = *((*pair)[0]);
  memory[stackPointer - 2] = *((*pair)[1]);
  stackPointer -= 2;
}

void CPU::popStackToRegisterPair(vector<uint8_t *> * pair)
{
  *((*pair)[0]) = memory[stackPointer + 1];
  *((*pair)[1]) = memory[stackPointer];
  stackPointer += 2;
}

void CPU::popStackToAccumulatorAndStatusPair()
{
  registerA = memory[stackPointer + 1];
  setStatusRegister(memory[stackPointer]);
  stackPointer += 2;
}

void CPU::setStatusRegister(uint8_t value)
{
  status = value;
  status |= 0x02;
  status &= 0xd7;
}

uint16_t CPU::valueOfRegisterPair(vector<uint8_t *> * pair)
{
  return *(*pair)[0] << 8 | *(*pair)[1];
}

void CPU::addValueToRegisterPairH(uint16_t value)
{
  vector<uint8_t *> * hlPair = registerPairMap[REGISTER_PAIR_H];
  uint16_t HLValue = *(*hlPair)[0] << 8 | *(*hlPair)[1];
  hasCarryAtBitIndex(HLValue, value, 15) ? setStatus(CARRY_BIT) : clearStatus(CARRY_BIT);

  uint16_t sum = HLValue + value;
  registerH = sum >> 8;
  registerL = sum & 0xff;
}

void CPU::incrementRegisterPair(vector<uint8_t *> * pair)
{
  uint16_t value = *(*pair)[0] << 8 | *(*pair)[1];
  value++;
  *(*pair)[0] = value >> 8;
  *(*pair)[1] = value & 0xff;
}

void CPU::decrementRegisterPair(vector<uint8_t *> * pair)
{
  uint16_t value = *(*pair)[0] << 8 | *(*pair)[1];
  value--;
  *(*pair)[0] = value >> 8;
  *(*pair)[1] = value & 0xff;
}

void CPU::exchangeRegisterPairs(vector<uint8_t *> * p1, vector<uint8_t *> * p2)
{
  uint8_t tempHighBits = *(*p1)[0];
  uint8_t tempLowBits = *(*p1)[1];
  *(*p1)[0] = *(*p2)[0];
  *(*p1)[1] = *(*p2)[1];
  *(*p2)[0] = tempHighBits;
  *(*p2)[1] = tempLowBits;
}

void CPU::exchangeRegistersAndMemory()
{
  uint8_t tempHighBits = registerL;
  uint8_t tempLowBits = registerH;
  registerL = memory[stackPointer];
  registerH = memory[stackPointer + 1];
  memory[stackPointer] = tempHighBits;
  memory[stackPointer + 1] = tempLowBits;
}

void CPU::handle3ByteOp(uint8_t opCode, uint8_t lowBytes, uint8_t highBytes)
{
  uint16_t bytes = highBytes << 8 | lowBytes;

  switch (opCode)
  {
    case LXI_B:
    case LXI_D:
    case LXI_H:
      replaceRegisterPair(registerPairFromOpCode(opCode), highBytes, lowBytes);
      break;
    case LXI_SP:
      stackPointer = bytes;
      break;
    case STA:
      memory[bytes] = registerA;
      break;
    case LDA:
      registerA = memory[bytes];
      break;
    case SHLD:
      memory[bytes] = registerL;
      memory[bytes + 1] = registerH;
      break;
    case LXLD:
      registerL = memory[bytes];
      registerH = memory[bytes + 1];
      break;
  }
}

void CPU::replaceRegisterPair(vector<uint8_t *> * pair, uint8_t highBytes, uint8_t lowBytes)
{
  *(*pair)[0] = highBytes;
  *(*pair)[1] = lowBytes;
}

void CPU::handle2ByteOp(uint8_t opCode, uint8_t value)
{
  switch (opCode)
  {
    case MVI_B:
    case MVI_C:
    case MVI_D:
    case MVI_E:
    case MVI_H:
    case MVI_L:
    case MVI_A:
      *(registerMap[opCode >> 3 & 7]) = value;
      break;
    case MVI_M:
      memory[currentMemoryAddress()] = value;
      break;
    case ADI:
      addValueToAccumulator(value, 0);
      break;  
    case ACI:
      addValueToAccumulator(value, carryBitSet() ? 1 : 0);
      break;  
    case SUI:
      subtractValueFromAccumulator(value);
      break;
    case SBI:
      subtractValueFromAccumulator(value + (carryBitSet() ? 1 : 0));
      break;
    case ANI:
      logicalANDWithAccumulator(value);
      break;
    case XRI:
      logicalXORWithAccumulator(value);
      break;
    case ORI:
      logicalORWithAccumulator(value);
      break;
    case CPI:
      compareValueToAccumulator(value);  
      break;
  }
}

uint16_t CPU::handleJumpByteOp()
{
  uint16_t address = registerH << 8 | registerL;
  return address;
}

uint16_t CPU::handleJump3ByteOp(uint8_t opCode, uint8_t lowBytes, uint8_t highBytes)
{
  uint16_t bytes = highBytes << 8 | lowBytes;
  uint16_t jumpMemoryLocation = 0;

  switch (opCode)
  {
    case JC:
      jumpMemoryLocation = carryBitSet() ? bytes : programCounter + 3;
      break;
    case JNC:
      jumpMemoryLocation = carryBitSet() ? programCounter + 3 : bytes;
      break;
    case JZ:
      jumpMemoryLocation = zeroBitSet() ? bytes : programCounter + 3;
      break;
    case JNZ:
      jumpMemoryLocation = zeroBitSet() ? programCounter + 3 : bytes;
      break;
    case JM:
      jumpMemoryLocation = signBitSet() ? bytes : programCounter + 3;
      break;
    case JP:
      jumpMemoryLocation = signBitSet() ? programCounter + 3 : bytes;
      break;
    case JPE:
      jumpMemoryLocation = parityBitSet() ? bytes : programCounter + 3;
      break;
    case JPO:
      jumpMemoryLocation = parityBitSet() ? programCounter + 3 : bytes;
      break;
    case JMP:
      jumpMemoryLocation = bytes;
      break;
  }

  return jumpMemoryLocation;
}

void CPU::push2ByteValueOnStack(uint16_t value)
{
  memory[stackPointer - 1] = value & 0xff;
  memory[stackPointer - 2] = value >> 8;
  stackPointer -= 2;
}

uint16_t CPU::handleCall3ByteOp(uint8_t opCode, uint8_t lowBytes, uint8_t highBytes)
{
  uint16_t bytes = highBytes << 8 | lowBytes;
  uint16_t jumpMemoryLocation = 0;

  switch (opCode)
  {
    case CALL:
      jumpMemoryLocation = performCallOperation(bytes);
      break;
    case CC:
      jumpMemoryLocation = carryBitSet() ? performCallOperation(bytes) : programCounter + 3;
      break;
    case CNC:
      jumpMemoryLocation = !carryBitSet() ? performCallOperation(bytes) : programCounter + 3;
      break;
    case CZ:
      jumpMemoryLocation = zeroBitSet() ? performCallOperation(bytes) : programCounter + 3;
      break;
    case CNZ:
      jumpMemoryLocation = !zeroBitSet() ? performCallOperation(bytes) : programCounter + 3;
      break;
    case CM:
      jumpMemoryLocation = signBitSet() ? performCallOperation(bytes) : programCounter + 3;
      break;
    case CP:
      jumpMemoryLocation = !signBitSet() ? performCallOperation(bytes) : programCounter + 3;
      break;
    case CPE:
      jumpMemoryLocation = parityBitSet() ? performCallOperation(bytes) : programCounter + 3;
      break;
    case CPO:
      jumpMemoryLocation = !parityBitSet() ? performCallOperation(bytes) : programCounter + 3;
      break;
  }

  return jumpMemoryLocation;
}

uint16_t CPU::performCallOperation(uint16_t memoryOffset)
{
    push2ByteValueOnStack(programCounter + 3);
    return memoryOffset;
}

uint16_t CPU::handleReturnOp(uint8_t opCode)
{
  uint16_t jumpMemoryLocation = 0;

  switch (opCode)
  {
    case RET:
      jumpMemoryLocation = pop2ByteValueFromStack();
      break;
    case RC:
      jumpMemoryLocation = carryBitSet() ? pop2ByteValueFromStack() : programCounter + 1;
      break;
    case RNC:
      jumpMemoryLocation = !carryBitSet() ? pop2ByteValueFromStack() : programCounter + 1;
      break;
    case RZ:
      jumpMemoryLocation = zeroBitSet() ? pop2ByteValueFromStack() : programCounter + 1;
      break;
    case RNZ:
      jumpMemoryLocation = !zeroBitSet() ? pop2ByteValueFromStack() : programCounter + 1;
      break;
    case RM:
      jumpMemoryLocation = signBitSet() ? pop2ByteValueFromStack() : programCounter + 1;
      break;
    case RP:
      jumpMemoryLocation = !signBitSet() ? pop2ByteValueFromStack() : programCounter + 1;
      break;
    case RPE:
      jumpMemoryLocation = parityBitSet() ? pop2ByteValueFromStack() : programCounter + 1;
      break;
    case RPO:
      jumpMemoryLocation = !parityBitSet() ? pop2ByteValueFromStack() : programCounter + 1;
      break;
  }

  return jumpMemoryLocation;
}

uint16_t CPU::pop2ByteValueFromStack()
{
  uint8_t lowBits = memory[stackPointer + 1];
  uint8_t highBits = memory[stackPointer];
  stackPointer += 2;

  return highBits << 8 | lowBits;
}

void CPU::handleInterrupt(uint8_t opCode)
{
  push2ByteValueOnStack(programCounter);
  interruptToHandle = opCode & 0x3f;
}

#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <map>
#include <vector>

const int MAX_MEMORY = 65536;

using namespace std;

class CPU
{
  public:
    CPU();
    bool carryBitSet();
    bool parityBitSet();
    bool signBitSet();
    bool zeroBitSet();
    bool auxiliaryCarryBitSet();
    bool allClear();
    void processProgram(uint8_t *program, uint16_t programSize);
    uint8_t registerA;
    uint8_t registerB;
    uint8_t registerC;
    uint8_t registerD;
    uint8_t registerE;
    uint8_t registerH;
    uint8_t registerL;
    uint8_t registerM();
    vector<uint8_t> memory;
    map<uint8_t, uint8_t *> registerMap;

  private:
    uint8_t status;
    void setStatus(uint8_t bit);
    void clearStatus(uint8_t bit);
    void flipStatusBit(uint8_t bit);
    void incrementRegister(uint8_t *reg);
    void incrementRegisterM();
    void decrementRegister(uint8_t *reg);
    void decrementRegisterM();
    uint16_t currentMemoryAddress();
    void setStatusFromRegister(uint8_t reg);
    void setParityBitFromRegister(uint8_t reg);
    void setZeroBitFromRegister(uint8_t reg);
    void setSignBitFromRegister(uint8_t reg);
    bool checkAuxiliaryCarryBitFromRegisterAndOperand(uint8_t reg, uint8_t operand);
    bool checkCarryBitFromRegisterAndOperand(uint8_t reg, uint8_t operand);
    void setAuxiliaryCarryBitFromRegisterAndOperand(uint8_t reg, uint8_t operand);
    void setCarryBitFromRegisterAndOperand(uint8_t reg, uint8_t operand);
    void complimentAccumulator();
    void decimalAdjustAccumulator();
    void moveRegisterToRegister(uint8_t opCode);
    void moveMemoryToAccumulator(uint8_t upperBitsAddress, uint8_t lowerBitsAddress);
    void moveAccumulatorToMemory(uint8_t upperbitsAddress, uint8_t lowerBitsAddress);
    void addValueToAccumulator(uint8_t opCode, uint8_t carry);
    uint8_t registerValueFromOpCode(uint8_t opCode);
    void subtractValueFromAccumulator(uint8_t value);
};

#endif

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
    bool followJumps;
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
    uint16_t stackPointer;
    uint8_t status;
    uint8_t *programCounter;
    uint8_t *programStart;
    uint8_t *programEnd;
    vector<uint8_t *> registerPairB;
    vector<uint8_t *> registerPairD;
    vector<uint8_t *> registerPairH;
    vector<uint8_t *> registerPairA;
    vector<uint8_t> memory;
    map<uint8_t, uint8_t *> registerMap;
    map<uint8_t, vector<uint8_t *> *> registerPairMap;

  private:
    void handleByteOp(uint8_t opCode);
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
    void addValueToAccumulator(uint8_t value, uint8_t carry);
    uint8_t registerValueFromOpCode(uint8_t opCode);
    void subtractValueFromAccumulator(uint8_t value);
    void logicalANDWithAccumulator(uint8_t value);
    void logicalXORWithAccumulator(uint8_t value);
    void logicalORWithAccumulator(uint8_t value);
    void compareValueToAccumulator(uint8_t value);
    void rotateAccumulatorLeft();
    void rotateAccumulatorRight();
    void rotateAccumulatorLeftWithCarry();
    void rotateAccumulatorRightWithCarry();
    vector<uint8_t *> * registerPairFromOpCode(uint8_t opCode);
    void pushRegisterPairOnStack(vector<uint8_t *> * pair);
    void popStackToRegisterPair(vector<uint8_t *> * pair);
    void popStackToAccumulatorAndStatusPair();
    void setStatusRegister(uint8_t value);
    uint16_t valueOfRegisterPair(vector<uint8_t *> * pair);
    void addValueToRegisterPairH(uint16_t value);
    void incrementRegisterPair(vector<uint8_t *> * pair);
    void decrementRegisterPair(vector<uint8_t *> * pair);
    void exchangeRegisterPairs(vector<uint8_t *> * p1, vector<uint8_t *> * p2);
    void exchangeRegistersAndMemory();
    void handle3ByteOp(uint8_t opCode, uint8_t lowBytes, uint8_t highBytes);
    void replaceRegisterPair(vector<uint8_t *> * pair, uint8_t highBytes, uint8_t lowBytes);
    void handle2ByteOp(uint8_t opCode, uint8_t value);
    uint8_t * handleJumpByteOp();
    uint8_t * handleJump3ByteOp(uint8_t opCode, uint8_t lowBytes, uint8_t highBytes);
};

#endif

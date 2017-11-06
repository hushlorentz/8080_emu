#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <vector>

const int MAX_MEMORY = 65536;

using namespace std;

class CPU
{
  public:
    CPU();
    bool carryBitSet();
    bool parityBitSet();
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

  private:
    uint8_t status;
    void setStatus(uint8_t bit);
    void clearStatus(uint8_t bit);
    void flipStatusBit(uint8_t bit);
    void addToRegister(uint8_t *reg, uint8_t operand);
    void addToRegisterM(uint16_t operand);
    uint16_t currentMemoryAddress();
    void setParityBitFrom8BitRegister(uint8_t reg);
    void setParityBitFrom16BitRegister(uint16_t reg);
};

#endif

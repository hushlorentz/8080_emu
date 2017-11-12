#include "bit_ops.h"

void setFlag(uint8_t *byte, uint8_t flag)
{
  *byte |= flag;
}

void clearFlag(uint8_t *byte, uint8_t flag)
{
  *byte &= ~flag;
}

bool hasFlag(uint8_t byte, uint8_t flag)
{
  return (byte & flag) == flag;
}

uint8_t getLowerNibble(uint8_t byte)
{
  return byte & 0x0F;
}

uint8_t getUpperNibble(uint8_t byte)
{
  return (byte >> 4) & 0x0F;
}

bool hasCarryAtBitIndex(uint16_t b1, uint16_t b2, uint16_t index)
{
  uint16_t masks[16] = {  0b0000000000000001,
                          0b0000000000000011,
                          0b0000000000000111,
                          0b0000000000001111,
                          0b0000000000011111,
                          0b0000000000111111,
                          0b0000000001111111,
                          0b0000000011111111,
                          0b0000000111111111,
                          0b0000001111111111,
                          0b0000011111111111,
                          0b0000111111111111,
                          0b0001111111111111,
                          0b0011111111111111,
                          0b0111111111111111,
                          0b1111111111111111 
  };

  uint16_t b1Slice = b1 & masks[index];
  uint16_t b2Slice = b2 & masks[index];

  return b1Slice + b2Slice > masks[index];
}

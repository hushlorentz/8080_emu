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

bool hasCarryAtBitIndex(uint8_t b1, uint8_t b2, uint8_t index)
{
  uint8_t masks[8] = {  0b00000001,
    0b00000011,
    0b00000111,
    0b00001111,
    0b00011111,
    0b00111111,
    0b01111111,
    0b11111111 };

  uint8_t b1Slice = b1 & masks[index];
  uint8_t b2Slice = b2 & masks[index];

  return b1Slice + b2Slice > masks[index];
}

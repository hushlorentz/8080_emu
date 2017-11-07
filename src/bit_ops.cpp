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

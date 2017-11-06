#include <cstdint>

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

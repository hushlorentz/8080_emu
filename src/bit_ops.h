#ifndef BIT_OPS_H
#define BIT_OPS_H

#include <cstdint>

void setFlag(uint8_t *byte, uint8_t flag);
void clearFlag(uint8_t *byte, uint8_t flag);
bool hasFlag(uint8_t byte, uint8_t flag);
uint8_t getLowerNibble(uint8_t byte);
uint8_t getUpperNibble(uint8_t byte);
bool hasCarryAtBitIndex(uint16_t b1, uint16_t b2, uint16_t index);

#endif

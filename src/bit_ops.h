#ifndef BIT_OPS_H
#define BIT_OPS_H

#include <cstdint>

void setFlag(uint8_t *byte, uint8_t flag);
void clearFlag(uint8_t *byte, uint8_t flag);
bool hasFlag(uint8_t byte, uint8_t flag);
uint8_t getLowerNibble(uint8_t byte);
uint8_t getUpperNibble(uint8_t byte);
bool hasCarryAtBitIndex(uint8_t b1, uint8_t b2, uint8_t index);

#endif

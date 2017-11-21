#include "space_invaders.h"

using namespace std;

SpaceInvaders::SpaceInvaders() : registerX(0), shiftOffset(0), inputRegister(0x8)
{
}

uint8_t SpaceInvaders::inputPortHandler(uint8_t address)
{
  switch (address)
  {
    case 0:
      return 1;
      break;
    case 1:
      return inputRegister;
      break;
    case 2:
      return 0;
      break;
    case 3:
      return registerX >> (8 - shiftOffset) & 0xff;
      break;
  }

  return 0;
}

uint8_t SpaceInvaders::outputPortHandler(uint8_t address, uint8_t value)
{
  switch (address)
  {
    case 2:
      shiftOffset = value & 0x7;
      break;
    case 4:
      registerX = registerX >> 8 | value << 8;
      break;
  }

  return 0;
}

void SpaceInvaders::buttonPressed(uint8_t button)
{
  inputRegister |= button;
}

void SpaceInvaders::buttonReleased(uint8_t button)
{
  inputRegister &= ~button;
}

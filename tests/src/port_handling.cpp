#include "../../src/space_invaders.h"

#include "catch.hpp"

using namespace Catch;

TEST_CASE("The Space Invaders port handlers") 
{
  SpaceInvaders invaders = SpaceInvaders();

  SECTION("Writing to port 2 sets the shift offset")
  {
    invaders.outputPortHandler(2, 0x04);

    REQUIRE(invaders.shiftOffset == 0x04);
  }

  SECTION("Reading From port 3 returns 8 bits of registerX shifted by shiftOffset")
  {
    invaders.outputPortHandler(4, 0xff);
    invaders.outputPortHandler(2, 0x02);

    REQUIRE(invaders.inputPortHandler(3) == 0xfc);
  }
   
  SECTION("Writing to port 4 shifts contents right 8 bits and ors the new value")
  {
    invaders.outputPortHandler(4, 0xff);

    REQUIRE(invaders.registerX == 0xff00);
  }
}

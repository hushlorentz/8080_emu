#include <cstdint>

#include "../../src/bit_ops.h"
#include "catch.hpp"

using namespace Catch;

TEST_CASE("Bit operations are performed correctly")
{
  SECTION("The lower nibble of 01010101 is 0101 (5)")
  {
    uint8_t byte = 0x55;

    REQUIRE(getLowerNibble(byte) == 0x5);
  }

  SECTION("The upper nibble of 10101010 is 1010 (10)")
  {
    uint8_t byte = 0xAA;

    REQUIRE(getUpperNibble(byte) == 0xA);
  }

  SECTION("Adding 0 and 1 does not produce a carry in the 0th position")
  {
    uint8_t b1 = 0x0;
    uint8_t b2 = 0x1;

    REQUIRE(!hasCarryAtBitIndex(b1, b2, 0));
  }

  SECTION("Adding 1 and 1 does produce a carry in the 0th position")
  {
    uint8_t b1 = 0x1;
    uint8_t b2 = 0x1;

    REQUIRE(hasCarryAtBitIndex(b1, b2, 0));
  }

  SECTION("Adding 1 and 1 does not produce a carry in the 1st position")
  {
    uint8_t b1 = 0x1;
    uint8_t b2 = 0x1;

    REQUIRE(!hasCarryAtBitIndex(b1, b2, 1));
  }

  SECTION("Adding 3 and 3 produces a carry in the 0th and 1st position")
  {
    uint8_t b1 = 0x03;
    uint8_t b2 = 0x03;

    REQUIRE(hasCarryAtBitIndex(b1, b2, 0));
    REQUIRE(hasCarryAtBitIndex(b1, b2, 1));
  }
}

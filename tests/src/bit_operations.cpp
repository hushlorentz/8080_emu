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
}

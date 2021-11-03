#include <catch2/catch.hpp>

TEST_CASE("Sample Test") {
  REQUIRE(5 == 5);
  REQUIRE((5 * 2) == 10);
  REQUIRE((5 + 2) == 7);
  REQUIRE(5 / 2 == 2);
}

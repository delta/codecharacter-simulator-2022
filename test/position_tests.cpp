#include "utils/game_map.hpp"
#include "utils/position.hpp"

#include <catch2/catch.hpp>

SCENARIO("Position::is_valid_spawn_position") {
  Map::no_of_cols = 64;
  Map::no_of_rows = 64;
  GIVEN("x and y coordinate") {
    WHEN("Coordinate is valid") {
      THEN("Returns true") {
        REQUIRE(Position::is_valid_spawn_position(0, 0) == true);
        REQUIRE(Position::is_valid_spawn_position(0, 10) == true);
        REQUIRE(Position::is_valid_spawn_position(63, 10) == true);
        REQUIRE(Position::is_valid_spawn_position(20, 0) == true);
        REQUIRE(Position::is_valid_spawn_position(20, 63) == true);
        REQUIRE(Position::is_valid_spawn_position(63, 63) == true);
      }
    }
    WHEN("The given coordinate is in bounds but invalid") {
      THEN("Returns false") {
        REQUIRE(Position::is_valid_spawn_position(1, 1) == false);
        REQUIRE(Position::is_valid_spawn_position(62, 1) == false);
        REQUIRE(Position::is_valid_spawn_position(2, 10) == false);
        REQUIRE(Position::is_valid_spawn_position(62, 60) == false);
        REQUIRE(Position::is_valid_spawn_position(46, 60) == false);
      }
    }
    WHEN("The given coordinate is out of bounds") {
      THEN("Returns false") {
        REQUIRE(Position::is_valid_spawn_position(-1, 0) == false);
        REQUIRE(Position::is_valid_spawn_position(0, -1) == false);
        REQUIRE(Position::is_valid_spawn_position(64, 1) == false);
        REQUIRE(Position::is_valid_spawn_position(1, 64) == false);
        REQUIRE(Position::is_valid_spawn_position(128, 128) == false);
        REQUIRE(Position::is_valid_spawn_position(128, 0) == false);
      }
    }
  }
}

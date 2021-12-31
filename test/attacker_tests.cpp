#include "../src/attacker/attacker.hpp"
#include <catch2/catch.hpp>

TEST_CASE("Attacker::move along straight horizontal line") {
  int range = 4;
  int speed = 2;
  Position initial_position(0, 0);
  Attacker::attribute_dictionary.clear();
  Attacker::attribute_dictionary.insert(
      std::make_pair(AttackerType::X, Attributes(0, range, 0, speed, 0)));
  Attacker attacker = Attacker::construct(0, AttackerType::X, initial_position);

  SECTION("when distance is more than range + speed") {
    int excess = 2;
    int distance = range + speed + excess;
    Position target = {distance, 0};
    attacker.move(target);

    auto final_position = attacker.get_position();
    REQUIRE(final_position.get_x() == excess);
  }

  SECTION("when distance is less than range + speed") {
    int distance = range + speed - 1;
    Position target = {distance, 0};
    attacker.move(target);

    auto final_position = attacker.get_position();
    REQUIRE(final_position.get_x() == distance - range);
  }

  SECTION("when target is already in range") {
    int distance = range - 1;
    Position target = {distance, 0};
    attacker.move(target);

    auto final_position = attacker.get_position();
    REQUIRE(final_position.get_x() == initial_position.get_x());
  }
}

TEST_CASE("Attacker::move along straight vertical line") {
  int range = 4;
  int speed = 2;
  Position initial_position(0, 0);
  Attacker::attribute_dictionary.clear();
  Attacker::attribute_dictionary.insert(
      std::make_pair(AttackerType::X, Attributes(0, range, 0, speed, 0)));
  Attacker attacker = Attacker::construct(0, AttackerType::X, initial_position);

  SECTION("when distance is more than range + speed") {
    int excess = 2;
    int distance = range + speed + excess;
    Position target = {0, distance};
    attacker.move(target);

    auto final_position = attacker.get_position();
    REQUIRE(final_position.get_y() == excess);
  }

  SECTION("when distance is less than range + speed") {
    int distance = range + speed - 1;
    Position target = {0, distance};
    attacker.move(target);

    auto final_position = attacker.get_position();
    REQUIRE(final_position.get_y() == distance - range);
  }

  SECTION("when target is already in range") {
    int distance = range - 1;
    Position target = {0, distance};
    attacker.move(target);

    auto final_position = attacker.get_position();
    REQUIRE(final_position.get_y() == initial_position.get_x());
  }
}

TEST_CASE("Attacker::move along arbitrary line") {
  int range = 10;
  int speed = 10;
  Position initial_position(0, 0);
  Attacker::attribute_dictionary.clear();
  Attacker::attribute_dictionary.insert(
      std::make_pair(AttackerType::X, Attributes(0, range, 0, speed, 0)));
  Attacker attacker = Attacker::construct(0, AttackerType::X, initial_position);

  SECTION("when distance is more than range + speed") {
    Position target = {15, 20}; // Right angle with hypotenuse 25
    attacker.move(target);

    auto final_position = attacker.get_position();

    // Should have moved 10 units along hypotenuse
    REQUIRE(final_position.get_x() == 6);
    REQUIRE(final_position.get_y() == 8);
  }

  SECTION("when distance is less than range + speed") {
    Position target = {9, 12}; // Right angle with hypotenuse 15
    attacker.move(target);

    auto final_position = attacker.get_position();

    // Should have moved 5 units along hypotenuse
    REQUIRE(final_position.get_x() == 3);
    REQUIRE(final_position.get_y() == 4);
  }

  SECTION("when target is already in range") {
    Position target = {3, 4}; // Right angle with hypotenuse 5
    attacker.move(target);

    auto final_position = attacker.get_position();
    REQUIRE(final_position.get_x() == initial_position.get_x());
  }
}

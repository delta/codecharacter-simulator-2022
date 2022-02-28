#include "attacker/attacker.hpp"
#include "defender/defender.hpp"

#include <catch2/catch.hpp>

SCENARIO("Attacker::move") {
  GIVEN("a straight horizontal trajectory") {
    int range = 4;
    int speed = 2;
    Position initial_position(0, 0);
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A1, Attributes(0, range, 0, speed, 0)));
    Attacker attacker = Attacker::construct(AttackerType::A1, initial_position);

    WHEN("distance is more than range + speed") {
      int excess = 3;
      int distance = range + speed + excess;
      Position target = {distance, 0};
      attacker.move(target);

      auto final_position = attacker.get_position();
      THEN("attacker moves only by speed units") {
        REQUIRE(final_position.get_x() == speed);
      }
    }

    WHEN("distance is less than range + speed") {
      int distance = range + speed - 1;
      Position target = {distance, 0};
      attacker.move(target);

      auto final_position = attacker.get_position();
      THEN("attacker moves to boundary of range") {
        REQUIRE(final_position.get_x() == distance - range);
      }
    }

    WHEN("target is already in range") {
      int distance = range - 1;
      Position target = {distance, 0};
      attacker.move(target);

      auto final_position = attacker.get_position();
      THEN("attacker does not move") {
        REQUIRE(final_position.get_x() == initial_position.get_x());
      }
    }
  }

  GIVEN("a straight vertical trajectory") {
    int range = 4;
    int speed = 2;
    Position initial_position(0, 0);
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A1, Attributes(0, range, 0, speed, 0)));
    Attacker attacker = Attacker::construct(AttackerType::A1, initial_position);

    WHEN("distance is more than range + speed") {
      int excess = 3;
      int distance = range + speed + excess;
      Position target = {0, distance};
      attacker.move(target);

      auto final_position = attacker.get_position();
      THEN("attacker moves only by speed units") {
        REQUIRE(final_position.get_y() == speed);
      }
    }

    WHEN("distance is less than range + speed") {
      int distance = range + speed - 1;
      Position target = {0, distance};
      attacker.move(target);

      auto final_position = attacker.get_position();
      THEN("attacker moves to boundary of range") {
        REQUIRE(final_position.get_y() == distance - range);
      }
    }

    WHEN("when target is already in range") {
      int distance = range - 1;
      Position target = {0, distance};
      attacker.move(target);

      auto final_position = attacker.get_position();
      THEN("attacker does not move") {
        REQUIRE(final_position.get_y() == initial_position.get_x());
      }
    }
  }

  GIVEN("an arbitrary line trajectory") {
    int range = 10;
    int speed = 10;
    Position initial_position(0, 0);
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A1, Attributes(0, range, 0, speed, 0)));
    Attacker attacker = Attacker::construct(AttackerType::A1, initial_position);

    WHEN("distance is more than range + speed") {
      Position target = {15, 20}; // Right angle with hypotenuse 25
      attacker.move(target);

      auto final_position = attacker.get_position();

      THEN("attacker moves only by speed units") {
        // Should have moved 10 units along hypotenuse
        REQUIRE(final_position.get_x() == 6);
        REQUIRE(final_position.get_y() == 8);
      }
    }

    WHEN("distance is less than range + speed") {
      Position target = {9, 12}; // Right angle with hypotenuse 15
      attacker.move(target);

      auto final_position = attacker.get_position();

      THEN("attacker moves to boundary of range") {
        // Should have moved 5 units along hypotenuse
        REQUIRE(final_position.get_x() == 3);
        REQUIRE(final_position.get_y() == 4);
      }
    }

    WHEN("target is already in range") {
      Position target = {3, 4}; // Right angle with hypotenuse 5
      attacker.move(target);

      auto final_position = attacker.get_position();
      THEN("attacker does not move") {
        REQUIRE(final_position.get_x() == initial_position.get_x());
      }
    }
  }
}

SCENARIO("Attacker::get_nearest_defender_index") {
  GIVEN("a list of defenders of varying length") {
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A1, Attributes(0, 0, 0, 0, 0)));
    Attacker attacker = Attacker::construct(AttackerType::A1, {0, 0});

    WHEN("the given list is empty") {
      std::vector<Defender> defenders;

      auto nearest_defender_index =
          attacker.get_nearest_defender_index(defenders);

      THEN("nearest defender does not exist") {
        REQUIRE(nearest_defender_index.has_value() == false);
      }
    }

    WHEN("list has defenders") {
      std::vector<Defender> defenders{
          Defender::construct(DefenderType::D1, {5, 0}),
          Defender::construct(DefenderType::D1, {3, 0}),
          Defender::construct(DefenderType::D1, {1, 0}),
          Defender::construct(DefenderType::D1, {7, 0}),
      };

      auto nearest_defender_index =
          attacker.get_nearest_defender_index(defenders);

      THEN("nearest defender is the closest defender") {
        REQUIRE(nearest_defender_index.has_value() == true);
        REQUIRE(nearest_defender_index.value() == 2);
      }
    }
  }
}

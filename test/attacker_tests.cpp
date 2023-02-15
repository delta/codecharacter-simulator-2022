#include "attacker/aerial_attacker/aerial_attacker.hpp"
#include "attacker/attacker.hpp"
#include "attacker/ground_attacker/ground_attacker.hpp"
#include "defender/aerial_defender/aerial_defender.hpp"
#include "defender/ground_defender/ground_defender.hpp"

#include <catch2/catch.hpp>

SCENARIO("Attacker::move") {
  GIVEN("a straight horizontal trajectory") {
    int range = 2;
    int speed = 4;
    Position initial_position(0, 0);
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(std::make_pair(
        AttackerType::A1, Attributes(0, range, 0, speed, 0, false)));
    Attacker *attacker =
        GroundAttacker::construct(AttackerType::A1, initial_position);

    WHEN("distance is more than range + speed") {
      int excess = 3;
      int distance = range + speed + excess;
      Position target = {distance, 0};
      attacker->move(target);

      auto final_position = attacker->get_position();
      THEN("attacker moves only by speed units") {
        REQUIRE(final_position.get_x() == speed);
      }
    }

    WHEN("distance is less than range + speed") {
      int shorterBy = 1;
      int distance = range + speed - shorterBy;
      Position target = {distance, 0};
      attacker->move(target);

      auto final_position = attacker->get_position();
      THEN("attacker moves to boundary of range") {
        REQUIRE(final_position.get_x() == distance - range);
      }
    }

    WHEN("target is already in range") {
      int distance = range - 1;
      Position target = {distance, 0};
      attacker->move(target);

      auto final_position = attacker->get_position();
      THEN("attacker does not move") {
        REQUIRE(final_position.get_x() == initial_position.get_x());
      }
    }
  }

  GIVEN("a straight vertical trajectory") {
    int range = 2;
    int speed = 4;
    Position initial_position(0, 0);
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(std::make_pair(
        AttackerType::A1, Attributes(0, range, 0, speed, 0, false)));
    Attacker *attacker =
        GroundAttacker::construct(AttackerType::A1, initial_position);

    WHEN("distance is more than range + speed") {
      int excess = 3;
      int distance = range + speed + excess;
      Position target = {0, distance};
      attacker->move(target);

      auto final_position = attacker->get_position();
      THEN("attacker moves only by speed units") {
        REQUIRE(final_position.get_y() == speed);
      }
    }

    WHEN("distance is less than range + speed") {
      int distance = range + speed - 1;
      Position target = {0, distance};
      attacker->move(target);

      auto final_position = attacker->get_position();
      THEN("attacker moves to boundary of range") {
        REQUIRE(final_position.get_y() == distance - range);
      }
    }

    WHEN("when target is already in range") {
      int distance = range - 1;
      Position target = {0, distance};
      attacker->move(target);

      auto final_position = attacker->get_position();
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
    Attacker::attribute_dictionary.insert(std::make_pair(
        AttackerType::A1, Attributes(0, range, 0, speed, 0, false)));
    Attacker *attacker =
        GroundAttacker::construct(AttackerType::A1, initial_position);

    WHEN("distance is more than range + speed") {
      Position target = {15, 20}; // Right angle with hypotenuse 25
      attacker->move(target);

      auto final_position = attacker->get_position();

      THEN("attacker moves only by speed units") {
        // Should have moved 10 units along hypotenuse
        REQUIRE(final_position.get_x() == 6);
        REQUIRE(final_position.get_y() == 8);
      }
    }

    WHEN("distance is less than range + speed") {
      Position target = {9, 12}; // Right angle with hypotenuse 15
      attacker->move(target);

      auto final_position = attacker->get_position();

      THEN("attacker moves to boundary of range") {
        // Should have moved 5 units along hypotenuse
        REQUIRE(final_position.get_x() == 3);
        REQUIRE(final_position.get_y() == 4);
      }
    }

    WHEN("target is already in range") {
      Position target = {3, 4}; // Right angle with hypotenuse 5
      attacker->move(target);

      auto final_position = attacker->get_position();
      THEN("attacker does not move") {
        REQUIRE(final_position.get_x() == initial_position.get_x());
      }
    }
  }
}

// FOR Non Aerial Attackers it can attack only Ground defenders that are in
// range

SCENARIO("Attacker::get_nearest_defender_index") {
  GIVEN("a list of defenders of varying length") {
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A1, Attributes(0, 0, 0, 0, 0, false)));
    Defender::attribute_dictionary.clear();
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D1, Attributes(0, 0, 0, 0, 0, false)));
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D5, Attributes(0, 0, 0, 0, 0, true)));
    Attacker *attacker = GroundAttacker::construct(AttackerType::A1, {0, 0});

    WHEN("the given list is empty") {
      std::vector<Defender *> defenders;

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);

      THEN("nearest defender does not exist") {
        REQUIRE(nearest_defender_index.has_value() == false);
      }
    }

    WHEN("list has only ground defenders") {
      std::vector<Defender *> defenders{
          GroundDefender::construct(DefenderType::D1, {5, 0}),
          GroundDefender::construct(DefenderType::D1, {3, 0}),
          GroundDefender::construct(DefenderType::D1, {1, 0}),
          GroundDefender::construct(DefenderType::D1, {7, 0}),
      };

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);

      THEN("nearest defender is the closest defender") {
        REQUIRE(nearest_defender_index.has_value() == true);
        REQUIRE(nearest_defender_index.value() == 2);
      }
    }

    WHEN("list has both aerial and ground defenders") {
      std::vector<Defender *> defenders{
          GroundDefender::construct(DefenderType::D1, {5, 0}),
          GroundDefender::construct(DefenderType::D1, {9, 0}),
          AerialDefender::construct(DefenderType::D5, {3, 0}),
          AerialDefender::construct(DefenderType::D5, {7, 0}),
          AerialDefender::construct(DefenderType::D1, {4, 0}),
          AerialDefender::construct(DefenderType::D5, {1, 0}),
          GroundDefender::construct(DefenderType::D1, {6, 0}),
          GroundDefender::construct(DefenderType::D1, {2, 0}),
      };

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);

      THEN("nearest defender is the closest ground defender") {
        REQUIRE(nearest_defender_index.has_value() == true);
        REQUIRE(nearest_defender_index.value() == 7);
      }
    }

    WHEN("list has only aerial defenders") {
      std::vector<Defender *> defenders{
          AerialDefender::construct(DefenderType::D5, {5, 0}),
          AerialDefender::construct(DefenderType::D5, {3, 0}),
          AerialDefender::construct(DefenderType::D5, {1, 0}),
          AerialDefender::construct(DefenderType::D5, {7, 0}),
      };

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);
      THEN("nearest defender does not exist") {
        REQUIRE(nearest_defender_index.has_value() == false);
      }
    }

    WHEN("list has only ground defenders with same distance") {
      std::vector<Defender *> defenders{
          GroundDefender::construct(DefenderType::D1, {5, 0}),
          GroundDefender::construct(DefenderType::D1, {3, 0}),
          GroundDefender::construct(DefenderType::D1, {3, 0}),
          GroundDefender::construct(DefenderType::D1, {7, 0}),
      };

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);

      THEN("nearest defender is the first defender with that distance") {
        REQUIRE(nearest_defender_index.has_value() == true);
        REQUIRE(nearest_defender_index.value() == 1);
      }
    }

    WHEN("list has both aerial and ground defenders with same distance") {
      std::vector<Defender *> defenders{
          GroundDefender::construct(DefenderType::D1, {5, 0}),
          AerialDefender::construct(DefenderType::D5, {3, 0}),
          GroundDefender::construct(DefenderType::D1, {3, 0}),
          AerialDefender::construct(DefenderType::D5, {7, 0})};

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);

      THEN("nearest ground defender is taken first") {
        REQUIRE(nearest_defender_index.has_value() == true);
        REQUIRE(nearest_defender_index.value() == 2);
      }
    }
  }
}

// FOR Aerial Attackers it can attack both Ground and Aerial defenders that are
// in range

SCENARIO("Attacker(Aerial)::get_nearest_defender_index") {
  GIVEN("a list of defenders of varying length") {
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A5, Attributes(0, 0, 0, 0, 0, true)));
    Attacker *attacker = AerialAttacker::construct(AttackerType::A5, {0, 0});
    Defender::attribute_dictionary.clear();
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D1, Attributes(0, 0, 0, 0, 0, false)));
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D5, Attributes(0, 0, 0, 0, 0, true)));

    WHEN("the given list is empty") {
      std::vector<Defender *> defenders;

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);

      THEN("nearest defender does not exist") {
        REQUIRE(nearest_defender_index.has_value() == false);
      }
    }

    WHEN("list has only ground defenders") {
      std::vector<Defender *> defenders{
          GroundDefender::construct(DefenderType::D1, {5, 0}),
          GroundDefender::construct(DefenderType::D1, {3, 0}),
          GroundDefender::construct(DefenderType::D1, {1, 0}),
          GroundDefender::construct(DefenderType::D1, {7, 0}),
      };

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);

      THEN("nearest defender is the closest defender") {
        REQUIRE(nearest_defender_index.has_value() == true);
        REQUIRE(nearest_defender_index.value() == 2);
      }
    }

    WHEN("list has both aerial and ground defenders") {
      std::vector<Defender *> defenders{
          GroundDefender::construct(DefenderType::D1, {5, 0}),
          GroundDefender::construct(DefenderType::D1, {9, 0}),
          AerialDefender::construct(DefenderType::D5, {3, 0}),
          AerialDefender::construct(DefenderType::D5, {7, 0}),
          GroundDefender::construct(DefenderType::D1, {4, 0}),
          AerialDefender::construct(DefenderType::D5, {2, 0}),
          GroundDefender::construct(DefenderType::D1, {6, 0}),
          AerialDefender::construct(DefenderType::D5, {1, 0}),
      };

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);

      THEN("nearest defender is the closest defender") {
        REQUIRE(nearest_defender_index.has_value() == true);
        REQUIRE(nearest_defender_index.value() == 7);
      }
    }

    WHEN("list has only aerial defenders") {
      std::vector<Defender *> defenders{
          AerialDefender::construct(DefenderType::D5, {5, 0}),
          AerialDefender::construct(DefenderType::D5, {3, 0}),
          AerialDefender::construct(DefenderType::D5, {1, 0}),
          AerialDefender::construct(DefenderType::D5, {7, 0}),
      };

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);

      THEN("nearest defender is taken first") {
        REQUIRE(nearest_defender_index.value() == 2);
      }
    }

    WHEN("list has only ground defenders with same distance") {
      std::vector<Defender *> defenders{
          GroundDefender::construct(DefenderType::D1, {5, 0}),
          GroundDefender::construct(DefenderType::D1, {3, 0}),
          GroundDefender::construct(DefenderType::D1, {3, 0}),
          GroundDefender::construct(DefenderType::D1, {7, 0}),
      };

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);

      THEN("nearest defender is the first defender taken") {
        REQUIRE(nearest_defender_index.has_value() == true);
        REQUIRE(nearest_defender_index.value() == 1);
      }
    }

    WHEN("list has only aerial defenders with same distance") {
      std::vector<Defender *> defenders{
          AerialDefender::construct(DefenderType::D5, {5, 0}),
          AerialDefender::construct(DefenderType::D5, {3, 0}),
          AerialDefender::construct(DefenderType::D5, {3, 0}),
          AerialDefender::construct(DefenderType::D5, {7, 0}),
      };

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);

      THEN("nearest first defender is taken") {
        REQUIRE(nearest_defender_index.has_value() == 1);
      }
    }

    WHEN("list has both aerial and ground defenders with same distance") {
      std::vector<Defender *> defenders{
          GroundDefender::construct(DefenderType::D1, {5, 0}),
          AerialDefender::construct(DefenderType::D5, {3, 0}),
          GroundDefender::construct(DefenderType::D1, {3, 0}),
          AerialDefender::construct(DefenderType::D5, {7, 0})};

      auto nearest_defender_index =
          attacker->get_nearest_defender_index(defenders);

      THEN("nearest defender is taken first") {
        REQUIRE(nearest_defender_index.has_value() == true);
        REQUIRE(nearest_defender_index.value() == 1);
      }
    }
  }
}

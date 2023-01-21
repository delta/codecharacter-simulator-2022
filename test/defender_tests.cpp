#include "attacker/aerial_attacker/aerial_attacker.hpp"
#include "attacker/attacker.hpp"
#include "attacker/ground_attacker/ground_attacker.hpp"
#include "defender/aerial_defender/aerial_defender.hpp"
#include "defender/ground_defender/ground_defender.hpp"

#include <catch2/catch.hpp>

SCENARIO("Defender::get_nearest_attacker_index") {
  GIVEN("a list of attackers of varying length") {
    Defender::attribute_dictionary.clear();
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D1, Attributes(0, 0, 0, 0, 0, false)));
    Defender *defender = GroundDefender::construct(DefenderType::D1, {0, 0});
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A1, Attributes(0, 0, 0, 0, 0, false)));
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A5, Attributes(0, 0, 0, 0, 0, true)));

    WHEN("the given list is empty") {
      std::vector<Attacker *> attackers;

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker does not exist") {
        REQUIRE(nearest_attacker_index.has_value() == false);
      }
    }

    WHEN("list has attackers") {
      std::vector<Attacker *> attackers{
          GroundAttacker::construct(AttackerType::A1, {5, 0}),
          GroundAttacker::construct(AttackerType::A1, {3, 0}),
          GroundAttacker::construct(AttackerType::A1, {1, 0}),
          GroundAttacker::construct(AttackerType::A1, {7, 0}),
      };

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker is the closest attacker") {
        REQUIRE(nearest_attacker_index.has_value() == true);
        REQUIRE(nearest_attacker_index.value() == 2);
      }
    }
  }
}

SCENARIO("Defender(Non Aerial)::get_nearest_attacker_index") {
  GIVEN("a list of attackers of varying length") {
    Defender::attribute_dictionary.clear();
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D1, Attributes(0, 0, 0, 0, 0, false)));
    Defender *defender = GroundDefender::construct(DefenderType::D1, {0, 0});
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A1, Attributes(0, 0, 0, 0, 0, false)));
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A5, Attributes(0, 0, 0, 0, 0, true)));

    WHEN("the given list is empty") {
      std::vector<Attacker *> attackers;

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker does not exist") {
        REQUIRE(nearest_attacker_index.has_value() == false);
      }
    }

    WHEN("list has only ground attackers") {
      std::vector<Attacker *> attackers{
          GroundAttacker::construct(AttackerType::A1, {5, 0}),
          GroundAttacker::construct(AttackerType::A1, {3, 0}),
          GroundAttacker::construct(AttackerType::A1, {1, 0}),
          GroundAttacker::construct(AttackerType::A1, {7, 0}),
      };

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker is the closest attacker") {
        REQUIRE(nearest_attacker_index.has_value() == true);
        REQUIRE(nearest_attacker_index.value() == 2);
      }
    }

    WHEN("list has only Aerial attackers") {
      std::vector<Attacker *> attackers{
          AerialAttacker::construct(AttackerType::A5, {5, 0}),
          AerialAttacker::construct(AttackerType::A5, {3, 0}),
          AerialAttacker::construct(AttackerType::A5, {4, 0}),
          AerialAttacker::construct(AttackerType::A5, {7, 0}),
      };

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker is null") {
        REQUIRE(nearest_attacker_index.has_value() == false);
      }
    }

    WHEN("list has both ground and Aerial attackers") {
      std::vector<Attacker *> attackers{
          GroundAttacker::construct(AttackerType::A1, {5, 0}),
          AerialAttacker::construct(AttackerType::A5, {1, 0}),
          GroundAttacker::construct(AttackerType::A1, {3, 0}),
          AerialAttacker::construct(AttackerType::A5, {7, 0}),
      };

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker is the closest ground attacker") {
        REQUIRE(nearest_attacker_index.has_value() == true);
        REQUIRE(nearest_attacker_index.value() == 2);
      }
    }

    WHEN("list has borh Aerial and Ground attackers with same distance") {
      std::vector<Attacker *> attackers{
          GroundAttacker::construct(AttackerType::A1, {6, 0}),
          AerialAttacker::construct(AttackerType::A5, {4, 0}),
          AerialAttacker::construct(AttackerType::A5, {4, 0}),
          GroundAttacker::construct(AttackerType::A1, {4, 0}),
      };

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker is the closest ground attacker that comes first "
           "in the list") {
        REQUIRE(nearest_attacker_index.has_value() == true);
        REQUIRE(nearest_attacker_index.value() == 3);
      }
    }
  }
}

SCENARIO("Defender(Aerial)::get_nearest_attacker_index") {
  GIVEN("a list of attackers of varying length") {
    Defender::attribute_dictionary.clear();
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D5, Attributes(0, 7, 0, 0, 0, true)));
    Defender *defender = AerialDefender::construct(DefenderType::D5, {0, 0});
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A1, Attributes(0, 0, 0, 0, 0, false)));
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A5, Attributes(0, 0, 0, 0, 0, true)));

    WHEN("the given list is empty") {
      std::vector<Attacker *> attackers;

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker does not exist") {
        REQUIRE(nearest_attacker_index.has_value() == false);
      }
    }

    WHEN("list has only ground attackers") {
      std::vector<Attacker *> attackers{
          GroundAttacker::construct(AttackerType::A1, {4, 0}),
          GroundAttacker::construct(AttackerType::A1, {7, 0}),
          GroundAttacker::construct(AttackerType::A1, {2, 0}),
          GroundAttacker::construct(AttackerType::A1, {5, 0}),
      };

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker is the nearest ground attacker") {
        REQUIRE(nearest_attacker_index.has_value() == true);
        REQUIRE(nearest_attacker_index.value() == 2);
      }
    }

    WHEN("list has only Aerial attackers") {
      std::vector<Attacker *> attackers{
          AerialAttacker::construct(AttackerType::A5, {5, 0}),
          AerialAttacker::construct(AttackerType::A5, {2, 0}),
          AerialAttacker::construct(AttackerType::A5, {4, 0}),
          AerialAttacker::construct(AttackerType::A5, {7, 0}),
      };

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker is the closest attacker") {
        REQUIRE(nearest_attacker_index.has_value() == true);
        REQUIRE(nearest_attacker_index.value() == 1);
      }
    }

    WHEN("list has both ground and Aerial attackers(Aerial Attacker is in "
         "range)") {
      std::vector<Attacker *> attackers{
          AerialAttacker::construct(AttackerType::A5, {5, 0}),
          GroundAttacker::construct(AttackerType::A1, {1, 0}),
          AerialAttacker::construct(AttackerType::A5, {3, 0}),
          AerialAttacker::construct(AttackerType::A5, {7, 0}),
      };

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker is the closest Aerial attacker that is "
           "closest(also in range) to "
           "the defender") {
        REQUIRE(nearest_attacker_index.has_value() == true);
        REQUIRE(nearest_attacker_index.value() == 2);
      }
    }

    WHEN("list has both ground and Aerial attackers(Aerial Attacker is not in "
         "range)") {
      std::vector<Attacker *> attackers{
          AerialAttacker::construct(AttackerType::A5, {8, 0}),
          GroundAttacker::construct(AttackerType::A1, {4, 0}),
          AerialAttacker::construct(AttackerType::A5, {9, 0}),
          GroundAttacker::construct(AttackerType::A1, {6, 0}),
      };

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker is the closest attacker (aerial or ground)") {
        REQUIRE(nearest_attacker_index.has_value() == true);
        REQUIRE(nearest_attacker_index.value() == 1);
      }
    }

    WHEN("list has both ground and Aerial attackers (Aerial Attacker is in "
         "range) with same distance") {
      std::vector<Attacker *> attackers{
          GroundAttacker::construct(AttackerType::A1, {6, 0}),
          AerialAttacker::construct(AttackerType::A5, {7, 0}),
          AerialAttacker::construct(AttackerType::A5, {7, 0}),
          GroundAttacker::construct(AttackerType::A1, {7, 0}),
      };

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker is the closest aerial attacker that comes first "
           "also in range "
           "in the list") {
        REQUIRE(nearest_attacker_index.has_value() == true);
        REQUIRE(nearest_attacker_index.value() == 1);
      }
    }

    WHEN("list has both ground and Aerial attackers (Aerial Attacker is not in "
         "range) with same distance") {
      std::vector<Attacker *> attackers{
          GroundAttacker::construct(AttackerType::A1, {9, 0}),
          AerialAttacker::construct(AttackerType::A5, {8, 0}),
          AerialAttacker::construct(AttackerType::A5, {9, 0}),
          GroundAttacker::construct(AttackerType::A1, {8, 0}),
      };

      auto nearest_attacker_index =
          defender->get_nearest_attacker_index(attackers);

      THEN("nearest attacker is the closest attacker (aerial or ground)") {
        REQUIRE(nearest_attacker_index.has_value() == true);
        REQUIRE(nearest_attacker_index.value() == 1);
      }
    }
  }
}

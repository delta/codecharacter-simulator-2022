#include "attacker/attacker.hpp"
#include "defender/defender.hpp"

#include <catch2/catch.hpp>

SCENARIO("Defender::get_nearest_attacker_index") {
  GIVEN("a list of attackers of varying length") {
    Defender::attribute_dictionary.clear();
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D1, Attributes(0, 0, 0, 0, 0)));
    Defender defender = Defender::construct(DefenderType::D1, {0, 0});

    WHEN("the given list is empty") {
      std::vector<Attacker> attackers;

      auto nearest_attacker_index =
          defender.get_nearest_attacker_index(attackers);

      THEN("nearest attacker does not exist") {
        REQUIRE(nearest_attacker_index.has_value() == false);
      }
    }

    WHEN("list has attackers") {
      std::vector<Attacker> attackers{
          Attacker::construct(AttackerType::A1, {5, 0}),
          Attacker::construct(AttackerType::A1, {3, 0}),
          Attacker::construct(AttackerType::A1, {1, 0}),
          Attacker::construct(AttackerType::A1, {7, 0}),
      };

      auto nearest_attacker_index =
          defender.get_nearest_attacker_index(attackers);

      THEN("nearest attacker is the closest attacker") {
        REQUIRE(nearest_attacker_index.has_value() == true);
        REQUIRE(nearest_attacker_index.value() == 2);
      }
    }
  }
}

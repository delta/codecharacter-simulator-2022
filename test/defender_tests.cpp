#include "attacker/attacker.hpp"
#include "defender/defender.hpp"

#include <catch2/catch.hpp>

SCENARIO("Defender::get_nearest_attacker_index") {
  GIVEN("a list of attackers of varying length") {
    Defender::attribute_dictionary.clear();
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D1, Attributes(0, 0, 0, 0, 0, true)));
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

SCENARIO("Ground Defender::get_nearest_attacker_index") {
  GIVEN("List of different types of attackers around") {
    WHEN("The list has both ground attackers and aerial attackers") {
      Attacker::attribute_dictionary.clear();
      Attacker::attribute_dictionary.insert(
          std::make_pair(AttackerType::A1, Attributes(0, 0, 0, 0, 0, true)));
      Attacker::attribute_dictionary.insert(
          std::make_pair(AttackerType::A2, Attributes(0, 0, 0, 0, 0, false)));
      std::vector<Attacker> attackers{
          Attacker::construct(AttackerType::A1, {0, 1}),
          Attacker::construct(AttackerType::A1, {0, 2}),
          Attacker::construct(AttackerType::A2, {0, 4}),
          Attacker::construct(AttackerType::A2, {0, 5}),
          Attacker::construct(AttackerType::A2, {0, 3}),
          Attacker::construct(AttackerType::A2, {0, 6}),
          Attacker::construct(AttackerType::A2, {0, 7})};

      Defender::attribute_dictionary.clear();
      Defender::attribute_dictionary.insert(
          std::make_pair(DefenderType::D1, Attributes(0, 0, 0, 0, 0, false)));
      Defender defender = Defender::construct(DefenderType::D1, {0, 0});

      auto nearest_attacker = defender.get_nearest_attacker_index(attackers);

      THEN("nearest ground attacker is the closet attacker") {
        REQUIRE(nearest_attacker.has_value() == true);
        REQUIRE(nearest_attacker.value() == 4);
      }
    }
  }
  WHEN("The list has only aerial attackers") {
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A1, Attributes(0, 0, 0, 0, 0, true)));
    std::vector<Attacker> attackers{
        Attacker::construct(AttackerType::A1, {0, 1}),
        Attacker::construct(AttackerType::A1, {0, 2}),
        Attacker::construct(AttackerType::A1, {0, 4}),
        Attacker::construct(AttackerType::A1, {0, 5}),
        Attacker::construct(AttackerType::A1, {0, 3}),
        Attacker::construct(AttackerType::A1, {0, 6}),
        Attacker::construct(AttackerType::A1, {0, 7})};

    Defender::attribute_dictionary.clear();
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D1, Attributes(0, 0, 0, 0, 0, false)));
    Defender defender = Defender::construct(DefenderType::D1, {0, 0});

    auto nearest_attacker = defender.get_nearest_attacker_index(attackers);

    THEN("nearest aerial attacker is the closet attacker") {
      REQUIRE(nearest_attacker.has_value() == true);
      REQUIRE(nearest_attacker.value() == 0);
    }
  }

  WHEN("The list has only ground attackers") {
    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A2, Attributes(0, 0, 0, 0, 0, false)));
    std::vector<Attacker> attackers{
        Attacker::construct(AttackerType::A2, {0, 1}),
        Attacker::construct(AttackerType::A2, {0, 2}),
        Attacker::construct(AttackerType::A2, {0, 4}),
        Attacker::construct(AttackerType::A2, {0, 5}),
        Attacker::construct(AttackerType::A2, {0, 3}),
        Attacker::construct(AttackerType::A2, {0, 6}),
        Attacker::construct(AttackerType::A2, {0, 7})};

    Defender::attribute_dictionary.clear();
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D1, Attributes(0, 0, 0, 0, 0, false)));
    Defender defender = Defender::construct(DefenderType::D1, {0, 0});

    auto nearest_attacker = defender.get_nearest_attacker_index(attackers);

    THEN("nearest ground attacker is the closet attacker") {
      REQUIRE(nearest_attacker.has_value() == true);
      REQUIRE(nearest_attacker.value() == 0);
    }
  }
}
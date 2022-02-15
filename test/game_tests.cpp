#include "defender/defender.hpp"
#include "game/game.hpp"
#include "utils/game_map.hpp"

#include <catch2/catch.hpp>

template <class T>
concept ActorBased = std::is_base_of<Actor, T>::value;

SCENARIO("Game::simulate") {

  GIVEN("The Map") {
    // helper function for getting defender by its position
    // concept

    auto find_actor_by_position =
        [&]<ActorBased T>(const std::vector<T> &actors,
                          Position p) -> std::optional<T> {
      auto it = std::ranges::find_if(
          actors, [&](const T &actor) { return actor.get_position() == p; });
      if (it == actors.end()) {
        return std::nullopt;
      }
      return std::make_optional(*it);
    };

    Map::no_of_rows = 10;
    Map::no_of_cols = 10;

    Map game_map({
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 2, 0, 2, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 2, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    });

    Defender::attribute_dictionary.clear();
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::X, Attributes(100, // hp
                                                   2,   // range
                                                   50,  // attack_power
                                                   0,   // speed
                                                   0    // price
                                                   )));
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::Y, Attributes(200, // hp
                                                   3,   // range
                                                   100, // attack_power
                                                   0,   // speed
                                                   0    // price
                                                   )));

    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::X, Attributes(100, // hp
                                                   2,   // range 2
                                                   50,  // attack_power
                                                   2,   // speed
                                                   100  // price
                                                   )));

    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::Y, Attributes(150, // hp
                                                   2,   // range
                                                   75,  // attack_power
                                                   3,   // speed
                                                   150  // price
                                                   )));
    std::vector<Defender> defenders_initial_state = game_map.spawn_defenders();
    Game game(std::vector<Attacker>{}, defenders_initial_state,
              1000 // number of coins
    );
    std::vector<std::pair<Position, AttackerType>> initial_spawn_positions{
        {{0, 4}, AttackerType::X}, // cost 100
        {{5, 0}, AttackerType::Y}, // cost 150
        {{5, 0}, AttackerType::Y}, // cost 150
        {{7, 0}, AttackerType::X}, // cost 100
        {{9, 4}, AttackerType::Y}  // cost 100
    };                             // total cost 650

    std::vector<std::pair<Position, AttackerType>> third_turn_spawn_pos = {
        {{9, 9}, AttackerType::Y},
        {{0, 0}, AttackerType::Y}}; // cost will be 150+150

    Game first_turn_state = game.simulate(initial_spawn_positions);
    Game second_turn_state = first_turn_state.simulate({});
    Game third_turn_state = second_turn_state.simulate(third_turn_spawn_pos);

    WHEN("FIRST TURN") {
      THEN("ONLY NEW ATTACKERS WILL BE SPAWNED IN THE GAME, ALL DEFENDERS ARE "
           "UNAFFECTED") {

        // Game new_game_state = game.simulate(initial_spawn_positions);

        // Defenders are unaffected because no attackers are currently in game,
        // they only spawned at the end of the simulation.
        REQUIRE(first_turn_state.get_defenders() == game.get_defenders());

        // The new state will have 650 lesser coins than prev game state because
        // of spawning attackers
        REQUIRE(first_turn_state.get_coins() == (game.get_coins() - 650));

        REQUIRE(first_turn_state.get_attackers() ==
                std::vector<Attacker>{
                    Attacker::construct(AttackerType::X, {0, 4}),
                    Attacker::construct(AttackerType::Y, {5, 0}),
                    Attacker::construct(AttackerType::Y, {5, 0}),
                    Attacker::construct(AttackerType::X, {7, 0}),
                    Attacker::construct(AttackerType::Y, {9, 4}),
                });
      }
    }
    WHEN("SECOND TURN") {
      /*
       *
       * Second Turn's Map state at the start of the simulation
       * in grid form
       *
       *
       * {0,			0,0 ,0,aX,0 ,0,0 ,0,0},
       * {0,		 	0,0 ,0,0 ,0 ,0,0 ,0,0},
       * {0,		 	0,0 ,0,0 ,0 ,0,0 ,0,0},
       * {0,		 	0,0 ,0,dX,0 ,0,0 ,0,0},
       * {0,		 	0,0 ,0,0 ,0 ,0,0 ,0,0},
       * {(2*aY), 0,dY,0,dY,dX,0,0 ,0,0},
       * {0,			0,0 ,0,dY,dX,0,0 ,0,0},
       * {aX		,	0,dX,0,0 ,0 ,0,dY,0,0},
       * {0,		 	0,0 ,0,0 ,0 ,0,0 ,0,0},
       * {0,		 	0,0 ,0,aY,0 ,0,0 ,0,0},
       *
       * aX has range 2 ,speed 2,attack_power 50,hp 100
       * aY has range 2 ,speed 3,attack_power 75,hp 150
       *
       * dX has range 2, attack_power 50, hp 100
       * dY has range 3, attack_power 100, hp 200
       *
       */

      enum AttackerIndex {
        A0 = 0, // Pos ->0,4
        A1,     // Pos -> 5,0
        A2,     // Pos -> 5,0
        A3,     // Pos -> 7,0
        A4      // Pos -> 9,4
      };

      THEN("Same amount of game coin as in prev turn") {
        REQUIRE(second_turn_state.get_coins() == first_turn_state.get_coins());
      }

      THEN("No attackers and defenders have died") {
        REQUIRE(second_turn_state.get_defenders().size() ==
                game.get_defenders().size());
        REQUIRE(second_turn_state.get_attackers().size() ==
                first_turn_state.get_attackers().size());
      }

      THEN("Destination is set for attackers and they're in moving state") {
        REQUIRE(std::ranges::all_of(std::array{A0, A4}, [&](size_t index) {
          return second_turn_state.get_attackers()[index]
                     .is_destination_set() &&
                 second_turn_state.get_attackers()[index].get_state() ==
                     Attacker::State::MOVING;
        }));
      }
      THEN("Some Attackers are already in range and in attacking state") {
        REQUIRE(std::ranges::all_of(std::array{A1, A2, A3}, [&](size_t index) {
          return second_turn_state.get_attackers()[index].get_state() ==
                 Attacker::State::ATTACKING;
        }));
      }

      THEN("Some defenders are in ATTACKING state") {
        const auto &defs = second_turn_state.get_defenders();
        REQUIRE(std::ranges::all_of(
            std::array{
                *find_actor_by_position(defs, {5, 2}),
                *find_actor_by_position(defs, {7, 2}),
                *find_actor_by_position(defs, {6, 4}),
            },
            [&](const Defender &defender) {
              return defender.get_state() == Defender::State::ATTACKING;
            }));
      }

      THEN("Attackers that were in range of Defenders lost some hp") {
        // A1, A3, A4 were in range of defenders so they lost some hp
        // A2 was in range as well but there were 2 attackers in range (for
        // defender at (5,2)) so it chose the attacker
        // which it found earlier and attacked that one, basically it chose
        // the one with smaller id value.
        //
        // If more than one Actor is in range of opposition Actor, then the
        // opposition Actor will choose the one which has smaller id. This
        // happens due to the usage of min_element in the get closest
        // functions which will return the first smallest element's iterator
        // it finds. And the way our simulation works, The attacker vector
        // will always be sorted by id field anyway hence implying that
        // min_element will return the element with smallest id value if there
        // are multiple min elements.

        REQUIRE(second_turn_state.get_attackers()[A0].get_hp() ==
                first_turn_state.get_attackers()[0].get_hp());

        REQUIRE(
            second_turn_state.get_attackers()[A1].get_hp() ==
            (first_turn_state.get_attackers()[A1].get_hp() -
             find_actor_by_position(second_turn_state.get_defenders(), {5, 2})
                 ->get_attack_power()));

        REQUIRE(second_turn_state.get_attackers()[A2].get_hp() ==
                first_turn_state.get_attackers()[A2].get_hp());

        // A3 attacked by defender at 7,2
        REQUIRE(
            second_turn_state.get_attackers()[A3].get_hp() ==
            (first_turn_state.get_attackers()[A3].get_hp() -
             find_actor_by_position(second_turn_state.get_defenders(), {7, 2})
                 ->get_attack_power()));

        REQUIRE(
            second_turn_state.get_attackers()[A4].get_hp() ==
            (first_turn_state.get_attackers()[A4].get_hp() -
             find_actor_by_position(second_turn_state.get_defenders(), {6, 4})
                 ->get_attack_power()));
      };

      THEN("Defenders that were in range of Attackers lost some hp") {
        const auto &fst_defs = first_turn_state.get_defenders();
        const auto &snd_defs = second_turn_state.get_defenders();
        const auto &snd_atckrs = second_turn_state.get_attackers();

        // was attacked by A1 and A2
        REQUIRE(find_actor_by_position(snd_defs, {5, 2})->get_hp() ==
                (find_actor_by_position(fst_defs, {5, 2})->get_hp() -
                 snd_atckrs[A1].get_attack_power() -
                 snd_atckrs[A2].get_attack_power()));

        REQUIRE(find_actor_by_position(snd_defs, {7, 2})->get_hp() ==
                (find_actor_by_position(fst_defs, {7, 2})->get_hp() -
                 snd_atckrs[A3].get_attack_power()));
      }
    }

    WHEN("THIRD TURN") {
      THEN("Game coin reduced by 300 from last turn") {
        REQUIRE(third_turn_state.get_coins() ==
                second_turn_state.get_coins() - 300);
      }
      THEN("Two new attackers spawned") {
        REQUIRE(std::array{third_turn_state.get_attackers().end()[-2],
                           third_turn_state.get_attackers().end()[-1]} ==
                std::array{
                    Attacker::construct(AttackerType::Y, {9, 9}),
                    Attacker::construct(AttackerType::Y, {0, 0}),
                });
      }

      THEN("Two defenders have died") {
        REQUIRE(third_turn_state.get_defenders().size() ==
                (second_turn_state.get_defenders().size() - 2));
        REQUIRE(
            !find_actor_by_position(third_turn_state.get_defenders(), {5, 2})
                 .has_value());
        REQUIRE(
            !find_actor_by_position(third_turn_state.get_defenders(), {7, 2})
                 .has_value());
      }

      THEN("Two Attackers have died") {
        auto num_of_attackers_in_second_turn =
            second_turn_state.get_attackers().size();
        // Dead attackers:
        //
        // - (5,0)[one with lower id value]
        //
        // - Attacker at (9,4)[initally it was in ( 9,4 ) but it would've moved
        // toward its nearest target which would be the defender at (7,2).
        // It will be in range of the defender at (5,2)[has range 2] and it will
        // die
        // ]
        auto dead_attackers = 2;
        auto spawned_attackers = 2;
        auto num_of_attackers_in_third_turn = num_of_attackers_in_second_turn -
                                              dead_attackers +
                                              spawned_attackers;
        REQUIRE(num_of_attackers_in_third_turn ==
                third_turn_state.get_attackers().size());

        // the attacker at 9,4 would've moved to 8,3 at the end of second turn,
        // trying to get in range with the defender at 7,2
        REQUIRE(
            !find_actor_by_position(third_turn_state.get_attackers(), {8, 3})
                 .has_value());
        // No of attackers at 5,0 is only 1 now as the other one would've died
        REQUIRE(
            std::ranges::count_if(
                third_turn_state.get_attackers(), [](const Attacker &attacker) {
                  return attacker.get_position() == Position{5, 0};
                }) == 1);
      }
    }
  }
}

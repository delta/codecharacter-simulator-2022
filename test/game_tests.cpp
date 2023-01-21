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
        {0, 0, 0, 0, 5, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 5, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    });

    Defender::attribute_dictionary.clear();
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D1, Attributes(100,  // hp
                                                    2,    // range
                                                    50,   // attack_power
                                                    0,    // speed
                                                    0,    // price
                                                    false // is_aerial
                                                    )));
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D2, Attributes(200,  // hp
                                                    3,    // range
                                                    100,  // attack_power
                                                    0,    // speed
                                                    0,    // price
                                                    false // is_aerial
                                                    )));
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType::D5, Attributes(200, // hp
                                                    3,   // range
                                                    100, // attack_power
                                                    0,   // speed
                                                    0,   // price
                                                    true // is_aerial
                                                    )));

    Attacker::attribute_dictionary.clear();
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A1, Attributes(100,  // hp
                                                    2,    // range 2
                                                    50,   // attack_power
                                                    2,    // speed
                                                    100,  // price
                                                    false // is_aerial
                                                    )));

    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A2, Attributes(150,  // hp
                                                    2,    // range
                                                    75,   // attack_power
                                                    3,    // speed
                                                    150,  // price
                                                    false // is_aerial
                                                    )));
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType::A5, Attributes(150, // hp
                                                    2,   // range
                                                    75,  // attack_power
                                                    3,   // speed
                                                    150, // price
                                                    true // is_aerial
                                                    )));
    std::vector<Defender *> defenders_initial_state =
        game_map.spawn_defenders();

    Game game(std::vector<Attacker *>{}, defenders_initial_state,
              1200 // number of coins
    );
    std::vector<std::pair<Position, AttackerType>> initial_spawn_positions{
        {{4, 0}, AttackerType::A1}, // cost 100
        {{0, 4}, AttackerType::A5}, // cost 150 //AERIAL
        {{0, 5}, AttackerType::A2}, // cost 150
        {{0, 7}, AttackerType::A1}, // cost 100
        {{4, 9}, AttackerType::A2}  // cost 150
    };                              // total cost 650

    std::vector<std::pair<Position, AttackerType>> second_turn_spawn_pos;

    std::vector<std::pair<Position, AttackerType>> third_turn_spawn_pos = {
        {{9, 9}, AttackerType::A5}, // AERIAL
        {{3, 3}, AttackerType::A2}, // this is invalid postion, so i'll be fined
                                    // only but the attacker wont be spawned,
                                    // but money is gone
        {{0, 0}, AttackerType::A2}}; // cost will be 150+150+150

    std::vector<std::pair<Position, AttackerType>> fourth_turn_spawn_pos;

    std::vector<std::pair<Position, AttackerType>> fifth_turn_spawn_pos;

    // FIRST TURN

    // First turn, no attackers would be there so no point in adding a target
    // If lets say somehow there's some requests for targets by the player in
    // first turn itself, that means it is invalid.
    std::unordered_map<Game::attacker_id, Game::defender_id>
        first_turn_player_set_targets{
            {1, 1},
            {2, 1}}; // attacker_id 1 and 2 both targetting defender_id 1

    Game first_turn_state =
        game.simulate(first_turn_player_set_targets, initial_spawn_positions);

    std::vector<Attacker> first_turn_attackers;
    std::vector<Defender> first_turn_defenders;

    std::ranges::for_each(first_turn_state.get_attackers(),
                          [&, index = 0](const Attacker *attacker) mutable {
                            first_turn_attackers.push_back(*attacker);
                          });

    std::ranges::for_each(first_turn_state.get_defenders(),
                          [&, index = 0](const Defender *defender) mutable {
                            first_turn_defenders.push_back(*defender);
                          });

    WHEN("FIRST TURN") {
      THEN("ONLY NEW ATTACKERS WILL BE SPAWNED IN THE GAME, ALL DEFENDERS ARE "
           "UNAFFECTED") {

        // Defenders are unaffected because no attackers are currently in game,
        // they only spawned at the end of the simulation.
        REQUIRE(first_turn_state.get_defenders() == game.get_defenders());

        // The new state will have 650 lesser coins than prev game state because
        // of spawning attackers
        REQUIRE(first_turn_state.get_coins() == (game.get_coins() - 650));

        // REQUIRE(first_turn_state.get_attackers() ==
        //         std::vector<Attacker>{
        //             Attacker::construct(AttackerType::A1, {4, 0}),
        //             Attacker::construct(AttackerType::A5, {0, 4}),
        //             Attacker::construct(AttackerType::A2, {0, 5}),
        //             Attacker::construct(AttackerType::A1, {0, 7}),
        //             Attacker::construct(AttackerType::A2, {4, 9}),
        //         });
      }
      THEN("All the custom target set requests are invalid, so no spawned "
           "attackers would have that field as true") {

        // std::cout << "here\n";

        // for (auto x : first_turn_state.get_attackers()) {
        //   std::cout << x->is_target_set_by_player() << "\n";
        //   if (x->is_target_set_by_player()) {
        //     std::cout << "set with " << x->get_target_id() << "\n";
        //   }
        // }

        // std::cout << "from here\n";

        REQUIRE(std::ranges::none_of(
            first_turn_state.get_attackers(), [](const Attacker *attacker) {
              // std::cout << attacker->is_target_set_by_player() << '\n';
              return attacker->is_target_set_by_player();
            }));
      }
    }

    // SECOND TURN

    // A4, located at 9,4 sets it target Defender at 6,4. Had this not been
    // done, it would set its destination to 7,2 which is nearer
    size_t manually_attacking_attackers_id =
        first_turn_state.get_attackers()[4]->get_id();
    size_t targetted_defenders_id =
        find_actor_by_position(first_turn_defenders, Position(4, 6))->get_id();
    std::unordered_map<Game::attacker_id, Game::defender_id>
        second_turn_player_set_targets = {
            {manually_attacking_attackers_id, targetted_defenders_id}};

    Game second_turn_state = first_turn_state.simulate(
        second_turn_player_set_targets, second_turn_spawn_pos);

    std::vector<Attacker> second_turn_attackers;
    std::vector<Defender> second_turn_defenders;

    std::ranges::for_each(second_turn_state.get_attackers(),
                          [&, index = 0](const Attacker *attacker) mutable {
                            second_turn_attackers.push_back(*attacker);
                          });

    std::ranges::for_each(second_turn_state.get_defenders(),
                          [&, index = 0](const Defender *defender) mutable {
                            second_turn_defenders.push_back(*defender);
                          });

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
       * {a5,		 	0,0 ,0,0 ,0 ,0,0 ,0,0},
       * {aY,     0,dY,0,dY,dX,0,0 ,0,0},
       * {0,			0,0 ,0,dY,dX,0,0 ,0,0},
       * {aX		,	0,dX,0,0 ,0 ,0,dY,0,0},
       * {0,		 	0,0 ,0,0 ,0 ,0,0 ,0,0},
       * {0,		 	0,0 ,0,a5,0 ,0,0 ,0,0},
       *
       * aX has range 2 ,speed 2,attack_power 50,hp 100
       * aY has range 2 ,speed 3,attack_power 75,hp 150
       * a5 has range 2 ,speed 3,attack_power 75,hp 150 //aerial
       *
       * dX has range 2, attack_power 50, hp 100
       * dY has range 3, attack_power 100, hp 200
       * d5 has range 3, attack_power 100, hp 200 //aerial
       *
       */

      enum AttackerIndex {
        A0 = 0, // Pos -> 4,0
        A1,     // Pos -> 0,5
        A2,     // Pos -> 0,5
        A3,     // Pos -> 0,7
        A4      // Pos -> 4,9
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

      THEN("Destination is set for attackers and they're in moving state, "
           "Destination setting takes into account manually set targets as "
           "well") {
        REQUIRE(std::ranges::all_of(std::array{A0, A4}, [&](size_t index) {
          return second_turn_state.get_attackers()[index]
                     ->is_destination_set() &&
                 second_turn_state.get_attackers()[index]->get_state() ==
                     Attacker::State::MOVING;
        }));

        REQUIRE(
            second_turn_state.get_attackers()[A4]->is_target_set_by_player());

        // Correctly set the destination
        REQUIRE(second_turn_state.get_attackers()[A4]->get_destination() ==
                Position(4, 6));
      }
      THEN("Some Attackers are already in range and in attacking state") {
        REQUIRE(std::ranges::all_of(std::array{A1, A2, A3}, [&](size_t index) {
          return second_turn_state.get_attackers()[index]->get_state() ==
                 Attacker::State::ATTACKING;
        }));
      }

      THEN("Some defenders are in ATTACKING state") {
        // const auto &defs = second_turn_state.get_defenders();
        REQUIRE(std::ranges::all_of(
            std::array{
                *find_actor_by_position(second_turn_defenders, {2, 5}),
                *find_actor_by_position(second_turn_defenders, {2, 7}),
                *find_actor_by_position(second_turn_defenders, {4, 6}),
            },
            [&](const Defender &defender) {
              return defender.get_state() == Defender::State::ATTACKING;
            }));
      }

      THEN("Attackers that were in range of Defenders lost some hp") {
        // A2,A3, A4 were in range of defenders.

        // std::cout <<
        // find_actor_by_position(second_turn_state.get_defenders(),
        //                                     {2, 5})
        //                  ->get_attack_power()
        //           << "\n";

        // std::cout << first_turn_state.get_attackers()[A2]->get_hp() << "\n";

        REQUIRE(second_turn_attackers[A0].get_hp() ==
                first_turn_attackers[0].get_hp());

        REQUIRE(second_turn_attackers[A2].get_hp() ==
                (first_turn_attackers[A2].get_hp() -
                 find_actor_by_position(second_turn_defenders, {2, 5})
                     ->get_attack_power()));

        // A1 is not at the minimum distance so it doesnt lose hp
        REQUIRE(second_turn_attackers[A1].get_hp() ==
                first_turn_attackers[A1].get_hp());

        // A3 attacked by defender at 2,7
        REQUIRE(second_turn_attackers[A3].get_hp() ==
                (first_turn_attackers[A3].get_hp() -
                 find_actor_by_position(second_turn_defenders, {2, 7})
                     ->get_attack_power()));

        REQUIRE(second_turn_attackers[A4].get_hp() ==
                (first_turn_attackers[A4].get_hp() -
                 find_actor_by_position(second_turn_defenders, {4, 6})
                     ->get_attack_power()));
      };

      THEN("Defenders that were in range of Attackers lost some hp") {
        // const auto &fst_defs = first_turn_state.get_defenders();
        // const auto &snd_defs = second_turn_state.get_defenders();
        // const auto &snd_atckrs = second_turn_state.get_attackers();

        // // was attacked by A1 and A2
        REQUIRE(
            find_actor_by_position(second_turn_defenders, {2, 5})->get_hp() ==
            (find_actor_by_position(first_turn_defenders, {2, 5})->get_hp() -
             second_turn_attackers[A1].get_attack_power() -
             second_turn_attackers[A2].get_attack_power()));

        REQUIRE(
            find_actor_by_position(second_turn_defenders, {2, 7})->get_hp() ==
            (find_actor_by_position(first_turn_defenders, {2, 7})->get_hp() -
             second_turn_attackers[A3].get_attack_power()));
      }
    }

    // THIRD TURN

    std::unordered_map<Game::attacker_id, Game::defender_id>
        third_turn_player_set_targets;
    Game third_turn_state = second_turn_state.simulate(
        third_turn_player_set_targets, third_turn_spawn_pos);

    std::vector<Attacker> third_turn_attackers;
    std::vector<Defender> third_turn_defenders;

    std::ranges::for_each(third_turn_state.get_attackers(),
                          [&, index = 0](const Attacker *attacker) mutable {
                            third_turn_attackers.push_back(*attacker);
                          });

    std::ranges::for_each(third_turn_state.get_defenders(),
                          [&, index = 0](const Defender *defender) mutable {
                            third_turn_defenders.push_back(*defender);
                          });

    WHEN("THIRD TURN") {
      THEN("Game coin reduced by 450 from last turn, 150 is fine for trying to "
           "spawn at invalid position") {
        REQUIRE(third_turn_state.get_coins() ==
                second_turn_state.get_coins() - 450);
      }

      THEN("Two new attackers spawned") {
        REQUIRE(std::array{third_turn_attackers.end()[-2],
                           third_turn_attackers.end()[-1]} ==
                std::array{
                    *AerialAttacker::construct(AttackerType::A5, {9, 9}),
                    *GroundAttacker::construct(AttackerType::A2, {0, 0}),
                });
      }

      THEN("Two defenders have died") {
        REQUIRE(third_turn_state.get_defenders().size() ==
                (second_turn_state.get_defenders().size() - 2));
        REQUIRE(
            !find_actor_by_position(third_turn_defenders, {2, 5}).has_value());
        REQUIRE(
            !find_actor_by_position(third_turn_defenders, {2, 7}).has_value());
      }

      THEN("Two Attackers have died") {
        auto num_of_attackers_in_second_turn =
            second_turn_state.get_attackers().size();
        // Dead attackers:
        //
        // - (0,5)
        //
        // - [0,7] will die as well
        //
        // - Attacker at (4,9)[initally it was in (4,9) but it would've moved
        // toward its set target which would be the defender at (4,6).
        // It will be in range of many defenders around the (4,6) area and it
        // would die
        // ]
        auto dead_attackers = 3;
        auto spawned_attackers = 2;
        auto num_of_attackers_in_third_turn = num_of_attackers_in_second_turn -
                                              dead_attackers +
                                              spawned_attackers;
        REQUIRE(num_of_attackers_in_third_turn ==
                third_turn_state.get_attackers().size());

        // The attacker that was manually attacking is gone
        REQUIRE(
            std::ranges::count_if(third_turn_attackers, [&](const Attacker &a) {
              return a.get_id() == manually_attacking_attackers_id;
            }) == 0);

        // attacker at 5,0 is dead
        REQUIRE(std::ranges::count_if(
                    third_turn_attackers, [](const Attacker &attacker) {
                      return attacker.get_position() == Position{0, 5};
                    }) == 0);

        // // The attacker at 7,0 is also no more
        REQUIRE(std::ranges::count_if(
                    third_turn_attackers, [](const Attacker &attacker) {
                      return attacker.get_position() == Position(0, 7);
                    }) == 0);
      }
    }

    // FOURTH TURN

    // const std::vector<Attacker *> attt = third_turn_state.get_attackers();

    const Attacker attacker_at_0_0 =
        *find_actor_by_position(third_turn_attackers, Position(0, 0));
    const Defender defender_at_4_5 =
        *find_actor_by_position(third_turn_defenders, Position(4, 5));
    std::unordered_map<Game::attacker_id, Game::defender_id>
        fourth_turn_player_set_targets = {
            {attacker_at_0_0.get_id(), defender_at_4_5.get_id()}};
    Game fourth_turn_state = third_turn_state.simulate(
        fourth_turn_player_set_targets, fourth_turn_spawn_pos);

    std::vector<Attacker> fourth_turn_attackers;
    std::vector<Defender> fourth_turn_defenders;

    std::ranges::for_each(fourth_turn_state.get_attackers(),
                          [&, index = 0](const Attacker *attacker) mutable {
                            fourth_turn_attackers.push_back(*attacker);
                          });

    std::ranges::for_each(fourth_turn_state.get_defenders(),
                          [&, index = 0](const Defender *defender) mutable {
                            fourth_turn_defenders.push_back(*defender);
                          });

    WHEN("FOURTH TURN") {
      THEN("Target is set for Player spawned at 0,0") {
        // The player spawned at 0,0 in 3rd turn is the last attacker in
        // attackers list
        auto player_spawned_at_0_0_in_3rd_turn =
            fourth_turn_attackers.end()[-1];
        REQUIRE(player_spawned_at_0_0_in_3rd_turn.is_target_set_by_player() ==
                true);
        REQUIRE(player_spawned_at_0_0_in_3rd_turn.get_destination() ==
                Position(4, 5));
      }
    }

    // FIFTH TURN
    std::unordered_map<Game::attacker_id, Game::defender_id>
        fifth_turn_player_set_targets = {
            {attacker_at_0_0.get_id(), defender_at_4_5.get_id()}};
    Game fifth_turn_state = fourth_turn_state.simulate(
        fourth_turn_player_set_targets, fourth_turn_spawn_pos);

    std::vector<Attacker> fifth_turn_attackers;
    std::vector<Defender> fifth_turn_defenders;

    std::ranges::for_each(fifth_turn_state.get_attackers(),
                          [&, index = 0](const Attacker *attacker) mutable {
                            fifth_turn_attackers.push_back(*attacker);
                          });

    std::ranges::for_each(fifth_turn_state.get_defenders(),
                          [&, index = 0](const Defender *defender) mutable {
                            fifth_turn_defenders.push_back(*defender);
                          });

    WHEN("FIFTH TURN") {
      THEN("Target is still set for Player which was spawned at 0,0 ") {
        // The player spawned at 0,0 in 3rd turn is still the last attacker in
        // attackers list in 5th turn because no new spawns
        auto player_spawned_at_0_0_in_3rd_turn = fifth_turn_attackers.end()[-1];
        REQUIRE(player_spawned_at_0_0_in_3rd_turn.is_target_set_by_player() ==
                true);
        REQUIRE(player_spawned_at_0_0_in_3rd_turn.get_destination() ==
                Position(4, 5));
      }
    }
  }
}

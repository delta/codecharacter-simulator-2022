#include "game/game.hpp"

#include <algorithm>

using namespace std;
Game::Game(std::vector<Attacker> attackers, std::vector<Defender> defenses)
    : _attackers(std::move(attackers)), _defenses(std::move(defenses)) {}

Game Game::simulate(
    const std::vector<std::pair<Position, AttackerType>> &spawn_positions) {

  const auto &prev_state_attackers = this->get_attackers();
  const auto &prev_state_defenders = this->get_defenders();

  std::vector<Attacker> attackers(prev_state_attackers.begin(),
                                  prev_state_attackers.end());
  std::vector<Defender> defenders(prev_state_defenders.begin(),
                                  prev_state_defenders.end());
  ranges::for_each(attackers,
                   [](Attacker &attacker) { attacker.clear_destination(); });

  // Attacker Loop
  ranges::for_each(
      prev_state_attackers, [&, index = 0](const Attacker &attacker) mutable {
        if (auto defender_index =
                attacker.get_nearest_defender_index(prev_state_defenders)) {
          if (attacker.is_in_range(defenders[*defender_index])) {
            attacker.attack(defenders[*defender_index]);
          } else {
            attackers[index].set_destination(
                defenders[*defender_index].get_position());
          }
        }
        index++;
      });

  // Defense loop
  ranges::for_each(
      prev_state_defenders, [&, index = 0](const Defender &defender) mutable {
        if (auto attacker_index =
                defender.get_nearest_attacker_index(prev_state_attackers)) {
          if (defender.is_in_range(attackers[*attacker_index])) {
            defender.attack(attackers[*attacker_index]);
            // set defender's state to ATTACKING
            defenders[index].set_state(Defender::State::ATTACKING);
          } else {
            // set defender's state to IDLE
            defenders[index].set_state(Defender::State::IDLE);
          }
        }
        index++;
      });

  // state update loop for attackers and defenders
  ranges::for_each(attackers,
                   [](Attacker &attacker) { attacker.update_state(); });
  ranges::for_each(defenders,
                   [](Defender &defender) { defender.update_state(); });

  // new attackers are spawned here
  ranges::for_each(spawn_positions, [&](const auto &spawn_details) {
    const auto &[position, attacker_type] = spawn_details;
    attackers.push_back(Attacker::construct(attacker_type, position));
  });

  return {move(attackers), move(defenders)};
}

const std::vector<Attacker> &Game::get_attackers() const {
  return this->_attackers;
}

const std::vector<Defender> &Game::get_defenders() const {
  return this->_defenses;
}

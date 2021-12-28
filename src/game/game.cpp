#include "game.hpp"

#include <algorithm>

using namespace std;
Game::Game(std::vector<Attacker> troops, std::vector<Defender> defenses)
    : _attackers(std::move(troops)), _defenses(std::move(defenses)) {}

Game Game::simulate(
    const std::vector<std::pair<Position, AttackerType>> &spawn_postions) {

  const auto &prev_state_attackers = this->get_attackers();
  const auto &prev_state_defenders = this->get_defenses();

  std::vector<Attacker> attackers(prev_state_attackers.begin(),
                                  prev_state_attackers.end());
  std::vector<Defender> defenders(prev_state_defenders.begin(),
                                  prev_state_defenders.end());
  ranges::for_each(attackers,
                   [](Attacker &attacker) { attacker.clear_destination(); });

  // Attacker Loop
  ranges::for_each(
      prev_state_attackers, [&, index = 0](const Attacker &attacker) mutable {
        auto pos = attacker.get_nearest_defense_postion(prev_state_defenders);
        size_t defender_index = this->get_attacker_from_position(pos);
        if (attacker.is_in_range(defenders[defender_index])) {
          attacker.attack(defenders[defender_index]);
        } else {
          attackers[index].set_destination(pos);
        }
        index++;
      });

  // Defense loop
  ranges::for_each(
      prev_state_defenders, [&, index = 0](const Defender &defender) mutable {
        auto pos = defender.get_nearest_attacker_postion(prev_state_attackers);
        size_t attacker_index = this->get_defender_from_position(pos);
        if (defender.is_in_range(attackers[attacker_index])) {
          defender.attack(attackers[attacker_index]);
          // set defender's state to ATTACKING
          defenders[index].set_state(Defender::State::ATTACKING);
        } else {
          // set defender's state to IDLE
          defenders[index].set_state(Defender::State::IDLE);
        }
        index++;
      });

  // state update loop for attackers and defenders
  ranges::for_each(attackers,
                   [](Attacker &attacker) { attacker.update_state(); });
  ranges::for_each(defenders,
                   [](Defender &defender) { defender.update_state(); });

  // new attackers are spawn here
  ranges::for_each(
      spawn_postions, [&](const pair<Position, AttackerType> &spawn_details) {
        attackers.push_back(
            Attacker::construct(spawn_details.second, spawn_details.first));
      });

  return {move(attackers), move(defenders)};
}

const std::vector<Attacker> &Game::get_attackers() const {
  return this->_attackers;
}

const std::vector<Defender> &Game::get_defenses() const {
  return this->_defenses;
}

[[nodiscard]] size_t Game::get_defender_from_position(Position p) const {
  auto it =
      ranges::find_if(this->get_defenses(), [&](const Defender &defender) {
        return defender.get_position() == p;
      });
  return distance(this->get_defenses().begin(), it);
}

[[nodiscard]] size_t Game::get_attacker_from_position(Position p) const {
  auto it =
      ranges::find_if(this->get_attackers(), [&](const Attacker &attacker) {
        return attacker.get_position() == p;
      });
  return distance(this->get_attackers().begin(), it);
}

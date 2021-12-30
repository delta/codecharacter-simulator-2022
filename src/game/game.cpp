#include "game.hpp"
#include "../attacker/attacker.hpp"
#include "../defender/defender.hpp"

#include <algorithm>

using namespace std;
Game::Game(std::vector<Attacker> attackers, std::vector<Defender> defenses,
           unsigned coins)
    : _attackers(std::move(attackers)), _defenses(std::move(defenses)),
      _coins(coins) {}

Game Game::simulate(
    const std::vector<std::tuple<size_t, Position, AttackerType>>
        &spawn_postions) {

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
      prev_state_attackers, [&, index = 0](const auto &attacker) mutable {
        auto pos = attacker.get_nearest_defender_postion(prev_state_defenders);
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
      prev_state_defenders, [&, index = 0](const auto &defender) mutable {
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

  // new attackers are spawned here
  ranges::for_each(spawn_postions, [&](const auto &spawn_details) {
    const auto &[attacker_id, position, attacker_type] = spawn_details;
    unsigned attacker_price =
        Attacker::attribute_dictionary[attacker_type].get_price();
    if (attacker_price > this->_coins) {
      this->_coins = 0;
      return;
    }
    this->_coins -= attacker_price;
    attackers.push_back(
        Attacker::construct(attacker_id, attacker_type, position));
  });

  return {move(attackers), move(defenders), _coins};
}

const std::vector<Attacker> &Game::get_attackers() const {
  return this->_attackers;
}

const std::vector<Defender> &Game::get_defenders() const {
  return this->_defenses;
}

[[nodiscard]] size_t Game::get_defender_from_position(Position p) const {
  auto it =
      ranges::find_if(this->get_defenders(), [&](const Defender &defender) {
        return defender.get_position() == p;
      });
  return distance(this->get_defenders().begin(), it);
}

[[nodiscard]] size_t Game::get_attacker_from_position(Position p) const {
  auto it =
      ranges::find_if(this->get_attackers(), [&](const Attacker &attacker) {
        return attacker.get_position() == p;
      });
  return distance(this->get_attackers().begin(), it);
}

[[nodiscard]] unsigned Game::get_coins() const { return this->_coins; }

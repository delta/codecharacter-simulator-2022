#include "game/game.hpp"
#include "logger/logger.hpp"

#include <algorithm>
#include <set>
#include <unordered_set>

using namespace std;

Game::Game(std::vector<Attacker> attackers, std::vector<Defender> defenders,
           unsigned coins)
    : _attackers(std::move(attackers)), _defenders(std::move(defenders)),
      _coins(coins) {
  // make all the id to index maps in constructor itself
  ranges::for_each(this->_attackers,
                   [&, index = 0](const Attacker &attacker) mutable {
                     this->_attacker_id_to_index[attacker.get_id()] = index++;
                   });
  ranges::for_each(this->_defenders,
                   [&, index = 0](const Defender &defender) mutable {
                     this->_defender_id_to_index[defender.get_id()] = index++;
                   });
}

Game Game::simulate(
    const std::unordered_map<attacker_id, defender_id> &player_set_targets,
    const std::vector<std::pair<Position, AttackerType>> &spawn_positions)
    const {

  const auto &prev_state_attackers = this->get_attackers();
  const auto &prev_state_defenders = this->get_defenders();

  std::vector<Attacker> attackers(prev_state_attackers.begin(),
                                  prev_state_attackers.end());
  std::vector<Defender> defenders(prev_state_defenders.begin(),
                                  prev_state_defenders.end());
  ranges::for_each(attackers,
                   [](Attacker &attacker) { attacker.clear_destination(); });

  ranges::for_each(
      player_set_targets,
      [&](const std::pair<attacker_id, defender_id> &entry) {
        auto attacker_index = this->get_attacker_index_by_id(entry.first);
        auto defender_index = this->get_defender_index_by_id(entry.second);
        // Getting the defender_index and checking its validity will ensure that
        // its a defender id that is currently in the map.
        if (attacker_index.has_value() && defender_index.has_value()) {
          attackers[*attacker_index].set_target(
              defenders[*defender_index].get_id());
        }
      });

  // Attacker Loop
  ranges::for_each(attackers, [&](Attacker &attacker) mutable {
    std::optional<index_t> defender_index{std::nullopt};
    if (attacker.is_target_set_by_player() &&
        this->get_defender_index_by_id(attacker.get_target_id())) {
      defender_index = this->get_defender_index_by_id(attacker.get_target_id());
    } else {
      // if it's here it shouldn't have a target or its target is invalid/doesnt
      // exist
      attacker.clear_target();
      defender_index =
          attacker.get_nearest_defender_index(prev_state_defenders);
    }

    if (defender_index.has_value()) {
      if ((attacker.is_in_range(defenders[*defender_index])) &&
          ((attacker.is_aerial_type()) ||
           (!defenders[*defender_index].is_aerial_type()))) {
        attacker.attack(defenders[*defender_index]);
      } else {
        attacker.set_destination(defenders[*defender_index].get_position());
      }
    }
  });

  // Defense loop
  ranges::for_each(defenders, [&](Defender &defender) mutable {
    if (auto attacker_index =
            defender.get_nearest_attacker_index(prev_state_attackers)) {
      if ((defender.is_in_range(attackers[*attacker_index])) &&
          ((defender.is_aerial_type()) ||
           (!attackers[*attacker_index].is_aerial_type()))) {
        defender.attack(attackers[*attacker_index]);
        // set defender's state to ATTACKING
        defender.set_state(Defender::State::ATTACKING);
      } else {
        // set defender's state to IDLE
        defender.set_state(Defender::State::IDLE);
      }
    }
  });

  // state update loop for attackers and defenders
  ranges::for_each(attackers,
                   [](Attacker &attacker) { attacker.update_state(); });

  // Stores the position of all the defenders that died at the end of this turn
  std::unordered_set<defender_id> dead_defender_ids;

  ranges::for_each(defenders, [&](Defender &defender) {
    defender.update_state();
    if (defender.get_state() == Defender::State::DEAD) {
      dead_defender_ids.insert(defender.get_id());
    }
  });

  // Clear the destination of all the attackers whose target is dead
  ranges::for_each(attackers, [&](Attacker &attacker) {
    if (attacker.is_target_set_by_player() &&
        dead_defender_ids.contains(attacker.get_target_id())) {
      // doing this so that next turn the player can be handled by the
      // simulation automatically if the player chooses not to set the target
      // manually again
      attacker.clear_target();
    }
  });

  // remove the dead attackers
  attackers.erase(remove_if(attackers.begin(), attackers.end(),
                            [](const Attacker &attacker) {
                              return attacker.get_state() ==
                                     Attacker::State::DEAD;
                            }),
                  attackers.end());

  // remove dead defenders
  defenders.erase(remove_if(defenders.begin(), defenders.end(),
                            [](const Defender &defender) {
                              return defender.get_state() ==
                                     Defender::State::DEAD;
                            }),
                  defenders.end());

  auto coins_left = this->get_coins();

  // new attackers are spawned here
  auto positions = std::set<Position>{};
  ranges::for_each(spawn_positions, [&](const auto &spawn_details) {
    const auto &[position, attacker_type] = spawn_details;
    const unsigned price = Attacker::attribute_dictionary[attacker_type].price;
    const bool is_aerial =
        Attacker::attribute_dictionary[attacker_type].is_aerial;
    if (price > coins_left) {
      return;
    }
    coins_left -= price;

    // if the position was valid then only it should add the attacker. Meaning
    // the price was deducted for the attacker from the coins_left but it wasnt
    // added. This is the penalty for trying to spawn at invalid position
    if (Position::is_valid_spawn_position(position.get_x(), position.get_y()) &&
        !positions.contains(position)) {
      positions.insert(position);
      attackers.push_back(Attacker::construct(attacker_type, position));
    }
  });

  return {std::move(attackers), std::move(defenders), coins_left};
}

const std::vector<Attacker> &Game::get_attackers() const {
  return this->_attackers;
}

const std::vector<Defender> &Game::get_defenders() const {
  return this->_defenders;
}

unsigned Game::get_coins() const { return this->_coins; }

std::optional<Game::index_t>
Game::get_attacker_index_by_id(attacker_id id) const {
  if (this->_attacker_id_to_index.contains(id)) {
    return this->_attacker_id_to_index.at(id);
  }
  return std::nullopt;
}
std::optional<Game::index_t>
Game::get_defender_index_by_id(defender_id id) const {
  if (this->_defender_id_to_index.contains(id)) {
    return this->_defender_id_to_index.at(id);
  }
  return std::nullopt;
}

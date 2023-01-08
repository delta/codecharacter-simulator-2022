#include "defender/defender.hpp"
#include "attacker/attacker.hpp"
#include "logger/logger.hpp"

#include <algorithm>
#include <optional>

Defender Defender::construct(DefenderType type, Position p) {
  Attributes attr = Defender::attribute_dictionary[type];
  return {type,          p, attr.hp, attr.range, attr.attack_power, attr.price,
          attr.is_aerial};
}

DefenderType Defender::get_type() const { return this->_type; }

void Defender::update_state() {
  if (this->get_hp() == 0) {
    this->set_state(State::DEAD);
    Logger::log_dead('D', this->_id);
  }
}

void Defender::attack(Actor &opponent) const {
  opponent.take_damage(this->get_attack_power());
  Logger::log_shoot('A', this->_id, opponent.get_id(), opponent.get_hp());
}

std::optional<size_t> Defender::get_nearest_attacker_index(
    const std::vector<Attacker> &attackers) const {
  if (attackers.empty()) {
    return std::nullopt;
  }

  // If it is Aerial Type we find both the nearest ground and aerial attacker
  // and return the nearest aerial attacker(if it exists and in range)
  //
  if (this->is_aerial_type()) {
    auto attacker = attackers.begin();
    bool aerial_attacker_exists = false;
    bool ground_attacker_exists = false;
    auto nearest_ground_attacker = attacker;
    auto nearest_aerial_attacker = attacker;
    while (attacker != attackers.end()) {
      if (attacker->is_aerial_type()) {
        if (aerial_attacker_exists) {
          if ((this->get_position().distance_to(attacker->get_position())) <
              (this->get_position().distance_to(
                  nearest_aerial_attacker->get_position()))) {
            nearest_aerial_attacker = attacker;
          }
        } else {
          aerial_attacker_exists = true;
          nearest_aerial_attacker = attacker;
        }
      } else {
        if (ground_attacker_exists) {
          if ((this->get_position().distance_to(attacker->get_position())) <
              (this->get_position().distance_to(
                  nearest_ground_attacker->get_position()))) {
            nearest_ground_attacker = attacker;
          }
        } else {
          ground_attacker_exists = true;
          nearest_ground_attacker = attacker;
        }
      }
      attacker++;
    }
    if (aerial_attacker_exists) {
      // if the nearest aerial attacker is in range then we return it
      if (this->is_in_range(*nearest_aerial_attacker)) {
        return std::distance(attackers.begin(), nearest_aerial_attacker);
      }
      // If aerial attacker exists but not in range, then we return the nearest
      // attacker irrespective of ground or aerial
      else {
        if (ground_attacker_exists &&
            (this->get_position().distance_to(
                 nearest_ground_attacker->get_position()) <
             this->get_position().distance_to(
                 nearest_aerial_attacker->get_position()))) {
          return std::distance(attackers.begin(), nearest_ground_attacker);
        } else {
          return std::distance(attackers.begin(), nearest_aerial_attacker);
        }
      }
    } else {
      // if no aerial attacker then loop is here only because ground attacker
      // exists
      // so just return nearest ground attacker
      return std::distance(attackers.begin(), nearest_ground_attacker);
    }
  }

  // If it is Ground Type we find the nearest ground attacker and return it
  else {
    auto attacker = attackers.begin();
    bool ground_attacker_exists = false;
    auto nearest_attacker = attacker;
    while (attacker != attackers.end()) {
      if (!attacker->is_aerial_type()) {
        if (ground_attacker_exists) {
          if ((this->get_position().distance_to(attacker->get_position())) <
              (this->get_position().distance_to(
                  nearest_attacker->get_position()))) {
            nearest_attacker = attacker;
          }
        } else {
          ground_attacker_exists = true;
          nearest_attacker = attacker;
        }
      }
      attacker++;
    }

    // if some ground attacker exists we return the nearest one
    if (ground_attacker_exists) {
      return std::distance(attackers.begin(), nearest_attacker);
    }

    // if no ground attacker exists then the defender cant attack anyone so
    // return nullopt
    else {
      return std::nullopt;
    }
  }

  return std::nullopt;
}

void Defender::set_state(State s) { this->_state = s; }

Defender::State Defender::get_state() const { return this->_state; }

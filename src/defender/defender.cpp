#include "defender/defender.hpp"
#include "attacker/attacker.hpp"
#include "logger/logger.hpp"

#include <algorithm>
#include <optional>

DefenderType Defender::get_type() const { return this->_type; }
Defender Defender::construct(DefenderType type, Position p) {
  Attributes attr = Defender::attribute_dictionary[type];
  return {type,
          p,
          attr.hp,
          attr.range,
          attr.attack_power,
          attr.price,
          attr.is_aerial,
          Defender::State::IDLE};
}

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
  if (this->is_aerial_type()) {
    auto nearest_attacker = std::min_element(
        attackers.begin(), attackers.end(),
        [this](const Attacker a, const Attacker b) {
          return this->get_position().distance_to(a.get_position()) <
                 this->get_position().distance_to(b.get_position());
        });
    return std::distance(attackers.begin(), nearest_attacker);
  } else {
    auto nearest_defender = std::min_element(
        attackers.begin(), attackers.end(),
        [this](const Attacker a, const Attacker b) {
          if (a.is_aerial_type() && !b.is_aerial_type()) {
            return false;
          }
          if (b.is_aerial_type() && !a.is_aerial_type()) {
            return true;
          }
          return this->get_position().distance_to(a.get_position()) <
                 this->get_position().distance_to(b.get_position());
        });
    return std::distance(attackers.begin(), nearest_defender);
  }

  return std::nullopt;
}

void Defender::set_state(State s) { this->_state = s; }

Defender::State Defender::get_state() const { return this->_state; }

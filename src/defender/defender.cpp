#include "defender/defender.hpp"
#include "attacker/attacker.hpp"
#include "logger/logger.hpp"

#include <optional>

Defender Defender::construct(DefenderType type, Position p) {
  Attributes attr = Defender::attribute_dictionary[type];
  return {type, p, attr.hp, attr.range, attr.attack_power, attr.price};
}

void Defender::update_state() {
  if (this->get_hp() == 0) {
    this->set_state(State::DEAD);
    Logger::log_dead('D', this->_id);
  }
}

std::optional<size_t> Defender::get_nearest_attacker_index(
    const std::vector<Attacker> &attackers) const {
  if (attackers.empty()) {
    return std::nullopt;
  }
  auto nearest_attacker = std::min_element(
      attackers.begin(), attackers.end(),
      [this](const Attacker &a, const Attacker &b) {
        return this->get_position().distance_to(a.get_position()) <
               this->get_position().distance_to(b.get_position());
      });
  return std::distance(attackers.begin(), nearest_attacker);
}

void Defender::set_state(State s) { this->_state = s; }

Defender::State Defender::get_state() const { return this->_state; }

DefenderType Defender::get_type() const { return this->_type; }

void Defender::log_shoot(size_t actor_id, size_t opponent_id,
                         unsigned new_opponent_hp) const {
  Logger::log_shoot('A', actor_id, opponent_id, new_opponent_hp);
}

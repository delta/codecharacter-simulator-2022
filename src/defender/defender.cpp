#include "defender.hpp"
#include "../attacker/attacker.hpp"
#include "../logger/logger.hpp"

Defender Defender::construct(size_t id, DefenderType type, Position p) {
  Attributes attr = Defender::attribute_dictionary[type];
  return {id, type, p, attr._hp, attr._range, attr._attack_power, attr._price};
}

void Defender::update_state() {
  if (this->get_hp() == 0) {
    this->set_state(State::DEAD);
    Logger::log_dead('D', this->_id);
  }
}

[[nodiscard]] Position Defender::get_nearest_attacker_postion(
    const std::vector<Attacker> &attackers) const {
  const Attacker &nearest_attacker = *std::min_element(
      attackers.begin(), attackers.end(),
      [this](const Attacker &a, const Attacker &b) {
        return this->get_position().distance_to(a.get_position()) <
               this->get_position().distance_to(b.get_position());
      });
  return nearest_attacker.get_position();
}

void Defender::set_state(State s) { this->_state = s; }

DefenderType Defender::get_type() const { return this->_type; }

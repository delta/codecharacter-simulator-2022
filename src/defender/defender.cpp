#include "defender/defender.hpp"
#include "attacker/attacker.hpp"

#include <optional>

Defender Defender::construct(DefenderType type, Position p) {
  Attributes attr = Defender::attribute_dictionary[type];
  return {type, p, attr._hp, attr._range, attr._attack_power, attr._price};
}

void Defender::update_state() {
  if (this->get_hp() == 0) {
    this->set_state(State::DEAD);
  }
}

[[nodiscard]] std::optional<size_t> Defender::get_nearest_attacker_id(
    const std::vector<Attacker> &attackers) const {
  if (attackers.empty()) {
    return std::nullopt;
  }
  const Attacker &nearest_attacker = *std::min_element(
      attackers.begin(), attackers.end(),
      [this](const Attacker &a, const Attacker &b) {
        return this->get_position().distance_to(a.get_position()) <
               this->get_position().distance_to(b.get_position());
      });
  return nearest_attacker.get_id();
}

void Defender::set_state(State s) { this->_state = s; }

DefenderType Defender::get_type() const { return this->_type; }

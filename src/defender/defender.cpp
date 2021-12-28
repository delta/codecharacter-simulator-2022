#include "defender.hpp"

Defender Defender::construct(DefenderType type, Position p) {
  Attributes attr = Defender::attribute_dictionary[type];
  return {p, attr._hp, attr._range, attr._attack_power, attr._price};
}

void Defender::update_state() {
  if (this->get_hp() == 0) {
    this->set_state(State::DEAD);
  }
}

void Defender::set_state(State s) { this->_state = s; }

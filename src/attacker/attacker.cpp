#include "attacker.hpp"

Attacker Attacker::construct(AttackerType type, Position p) {
  Attributes attr = Attacker::attribute_dictionary[type];
  return {type,        p,
          attr._hp,    attr._range,
          attr._speed, attr._attack_power,
          attr._price, Attacker::State::SPAWNED};
}

bool Attacker::is_destination_set() const { return this->_is_dest_set; }

void Attacker::clear_destination() { this->_is_dest_set = false; }

void Attacker::set_destination(Position p) {
  this->_is_dest_set = true;
  this->_destination = p;
}

Position Attacker::get_destination() { return this->_destination; }

void Attacker::update_state() {
  if (this->is_destination_set()) {
    this->set_state(State::MOVING);
    this->move(this->get_destination());
  } else {
    this->set_state(State::ATTACKING);
  }

  if (this->get_hp() == 0) {
    this->set_state(State::DEAD);
  }
}

void Attacker::set_state(State s) { this->_state = s; }

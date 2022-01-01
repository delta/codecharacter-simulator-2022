#include <cmath>

#include "attacker/attacker.hpp"
#include "logger/logger.hpp"

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

void Attacker::move(Position position) {
  auto current_position = this->_position;
  auto distance = current_position.distance_to(position);
  double angle = atan((double)(position.get_y() - current_position.get_y()) /
                      (position.get_x() - current_position.get_x()));
  double step = this->_speed;
  if (distance < this->_range) {
    // Already in range
    return;
  }
  if (this->_speed > distance - this->_range) {
    // Move only to boundary of range
    step = distance - this->_range;
  }
  int x_step = round(step * cos(angle));
  int y_step = round(step * sin(angle));
  this->_position = Position(current_position.get_x() + x_step,
                             current_position.get_y() + y_step);
}

void Attacker::set_state(State s) { this->_state = s; }

AttackerType Attacker::get_type() const { return this->_type; }

#include <cmath>

#include "../logger/logger.hpp"

#include "attacker.hpp"

Attacker Attacker::construct(size_t id, AttackerType type, Position p) {
  Attributes attr = Attacker::attribute_dictionary[type];
  return {id,
          type,
          p,
          attr._hp,
          attr._range,
          attr._speed,
          attr._attack_power,
          attr._price,
          Attacker::State::SPAWNED};
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
    Logger::log_move(this->_id, this->get_destination().get_x(),
                     this->get_destination().get_y());
  } else {
    this->set_state(State::ATTACKING);
  }

  if (this->get_hp() == 0) {
    this->set_state(State::DEAD);
    Logger::log_dead('A', this->_id);
  }
}

[[nodiscard]] Position Attacker::get_nearest_defender_postion(
    const std::vector<Defender> &defenders) const {
  const Defender &nearest_defender = *std::min_element(
      defenders.begin(), defenders.end(),
      [this](const Defender &a, const Defender &b) {
        return this->get_position().distance_to(a.get_position()) <
               this->get_position().distance_to(b.get_position());
      });
  return nearest_defender.get_position();
}

void Attacker::move(Position position) {
  auto current_position = this->_position;
  auto distance = current_position.distance_to(position);
  double angle = atan((double)(position.get_y() - current_position.get_y()) /
                      (position.get_x() - current_position.get_x()));
  double step = this->_speed;
  if (this->_speed > distance - this->_attack_power) {
    step = this->_speed + this->_attack_power - distance;
  }
  double x_step = step * cos(angle);
  double y_step = step * sin(angle);
  this->_position = Position(current_position.get_x() + (int)x_step,
                             current_position.get_y() + (int)y_step);
}

void Attacker::set_state(State s) { this->_state = s; }

AttackerType Attacker::get_type() const { return this->_type; }

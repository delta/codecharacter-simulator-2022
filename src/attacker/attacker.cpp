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

void Attacker::move(Position position) { this->_position = position; }

void Attacker::set_state(State s) { this->_state = s; }

AttackerType Attacker::get_type() const { return this->_type; }

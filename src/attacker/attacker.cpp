#include <cmath>
#include <optional>

#include "attacker/attacker.hpp"
#include "logger/logger.hpp"
#include "utils/game_map.hpp"

Attacker Attacker::construct(AttackerType type, Position p) {
  Attributes attr = Attacker::attribute_dictionary[type];
  Logger::log_spawn(_id_counter, type, p.get_x(), p.get_y());
  return {type,       p,
          attr.hp,    attr.range,
          attr.speed, attr.attack_power,
          attr.price, Attacker::State::SPAWNED};
}

bool Attacker::is_destination_set() const { return this->_is_dest_set; }

void Attacker::clear_destination() { this->_is_dest_set = false; }

void Attacker::set_destination(Position p) {
  this->_is_dest_set = true;
  this->_destination = p;
}

Position Attacker::get_destination() const { return this->_destination; }

bool Attacker::is_target_set_by_player() const {
  return this->_is_target_set_by_player;
}

size_t Attacker::get_target_id() const { return this->_target_id; }

void Attacker::set_target(size_t defender_id) {
  this->_is_target_set_by_player = true;
  this->_target_id = defender_id;
}

void Attacker::clear_target() { this->_is_target_set_by_player = false; }

void Attacker::update_state() {
  if (this->is_destination_set()) {
    this->set_state(State::MOVING);
    this->move(this->get_destination());
  } else {
    this->set_state(State::ATTACKING);
  }

  if (this->get_hp() == 0) {
    this->set_state(State::DEAD);
    Logger::log_dead('A', this->_id);
  }
}

[[nodiscard]] std::optional<size_t> Attacker::get_nearest_defender_index(
    const std::vector<Defender> &defenders) const {
  if (defenders.empty()) {
    return std::nullopt;
  }
  auto nearest_defender = std::min_element(
      defenders.begin(), defenders.end(),
      [this](const Defender &a, const Defender &b) {
        return this->get_position().distance_to(a.get_position()) <
               this->get_position().distance_to(b.get_position());
      });
  return std::distance(defenders.begin(), nearest_defender);
}

void Attacker::move(Position position) {
  auto current_position = this->_position;
  auto distance = current_position.distance_to(position);
  auto delta_y = position.get_y() - current_position.get_y();
  auto delta_x = position.get_x() - current_position.get_x();
  double angle = atan2(delta_y, delta_x);
  double step = this->_speed;
  if (distance < this->get_range()) {
    // Already in range
    return;
  }
  if (this->_speed > distance - ((int)this->get_range())) {
    // Move only to boundary of range
    step = std::max(distance - (int)this->get_range(), 0.0);
  }
  int x_step = round(step * cos(angle));
  int y_step = round(step * sin(angle));

  int new_x = std::clamp(current_position.get_x() + x_step, 1,
                         (int)Map::no_of_cols - 1);
  int new_y = std::clamp(current_position.get_y() + y_step, 1,
                         (int)Map::no_of_rows - 1);
  this->_position = Position(new_x, new_y);

  Logger::log_move(this->_id, this->_position.get_x(), this->_position.get_y());
}

void Attacker::attack(Actor &opponent) const {
  opponent.take_damage(this->get_attack_power());
  Logger::log_shoot('D', this->_id, opponent.get_id(), opponent.get_hp());
}

void Attacker::set_state(State s) { this->_state = s; }

AttackerType Attacker::get_type() const { return this->_type; }

Attacker::State Attacker::get_state() const { return this->_state; }

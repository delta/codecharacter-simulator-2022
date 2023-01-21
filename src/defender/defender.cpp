#include "defender/defender.hpp"
#include "attacker/attacker.hpp"
#include "logger/logger.hpp"

#include <algorithm>
#include <optional>

DefenderType Defender::get_type() const { return this->_type; }

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
    const std::vector<Attacker *> &attackers) const {
  return std::nullopt;
}

void Defender::set_state(State s) { this->_state = s; }

Defender::State Defender::get_state() const { return this->_state; }

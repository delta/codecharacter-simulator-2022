#include "ground_attacker.hpp"
#include "defender/defender.hpp"
#include "logger/logger.hpp"

#include <algorithm>
#include <optional>

Attacker *GroundAttacker::construct(AttackerType type, Position p) {
  Attributes attr = Attacker::attribute_dictionary[type];
  Attacker *attacker = new GroundAttacker(
      type, p, attr.hp, attr.range, attr.speed, attr.attack_power, attr.price,
      attr.is_aerial, Attacker::State::SPAWNED);
  Logger::log_spawn(attacker->get_id(), type, p.get_x(), p.get_y());
  return attacker;
}

[[nodiscard]] std::optional<size_t> GroundAttacker::get_nearest_defender_index(
    const std::vector<Defender *> &defenders) const {

  if (defenders.empty()) {
    return std::nullopt;
  }
  auto nearest_defender = std::min_element(
      defenders.begin(), defenders.end(),
      [this](const Defender *a, const Defender *b) {
        // ground defenders are given prefernece
        if (a->is_aerial_type() && !b->is_aerial_type())
          return false;
        if (b->is_aerial_type() && !a->is_aerial_type())
          return true;
        // if both are of same type nearest one is returned
        return this->get_position().distance_to(a->get_position()) <
               this->get_position().distance_to(b->get_position());
      });
  // Handle absence of grpund defenders
  if (!(*nearest_defender)->is_aerial_type()) {
    return std::distance(defenders.begin(), nearest_defender);
  }
  return std::nullopt;
}

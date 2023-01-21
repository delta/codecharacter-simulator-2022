#include "ground_defender.hpp"
#include "attacker/attacker.hpp"

#include <algorithm>
#include <optional>

Defender *GroundDefender::construct(DefenderType type, Position p) {
  Attributes attr = Defender::attribute_dictionary[type];
  return new GroundDefender(type, p, attr.hp, attr.range, attr.attack_power,
                            attr.price, attr.is_aerial, Defender::State::IDLE);
}

[[nodiscard]] std::optional<size_t> GroundDefender::get_nearest_attacker_index(
    const std::vector<Attacker *> &attackers) const {
  if (attackers.empty()) {
    return std::nullopt;
  }

  auto nearest_attacker = std::min_element(
      attackers.begin(), attackers.end(),
      [this](const Attacker *a, const Attacker *b) {
        if (a->is_aerial_type() && !b->is_aerial_type())
          return false;
        if (b->is_aerial_type() && !a->is_aerial_type())
          return true;
        return this->get_position().distance_to(a->get_position()) <
               this->get_position().distance_to(b->get_position());
      });
  if (!(*nearest_attacker)->is_aerial_type()) {
    return std::distance(attackers.begin(), nearest_attacker);
  }
  return std::nullopt;
}
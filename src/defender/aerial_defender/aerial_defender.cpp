#include "aerial_defender.hpp"
#include "attacker/attacker.hpp"

#include <algorithm>
#include <optional>

Defender *AerialDefender::construct(DefenderType type, Position p) {
  Attributes attr = Defender::attribute_dictionary[type];
  return new AerialDefender(type, p, attr.hp, attr.range, attr.attack_power,
                            attr.price, attr.is_aerial, Defender::State::IDLE);
}

[[nodiscard]] std::optional<size_t> AerialDefender::get_nearest_attacker_index(
    const std::vector<Attacker *> &attackers) const {
  if (attackers.empty()) {
    return std::nullopt;
  }

  auto nearest_attacker = std::min_element(
      attackers.begin(), attackers.end(),
      [this](const Attacker *a, const Attacker *b) {
        // aerial defenders in range are given preference
        if (a->is_aerial_type() && !b->is_aerial_type() &&
            this->is_in_range(*a))
          return true;
        if (b->is_aerial_type() && !a->is_aerial_type() &&
            this->is_in_range(*b))
          return false;
        // if both of same type nearest one is returned
        return this->get_position().distance_to(a->get_position()) <
               this->get_position().distance_to(b->get_position());
      });

  return std::distance(attackers.begin(), nearest_attacker);
}

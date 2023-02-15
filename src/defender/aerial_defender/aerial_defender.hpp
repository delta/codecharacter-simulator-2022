#pragma once

#include "actor/actor.hpp"
#include "defender/defender.hpp"
#include "utils/attributes.hpp"
#include "utils/position.hpp"

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

class Attacker;

class AerialDefender : public Defender {
public:
  AerialDefender(DefenderType type, Position position, unsigned hp,
                 unsigned range, unsigned attack_power, unsigned price,
                 bool is_aerial, State state = State::IDLE)
      : Defender{type,         position, hp,        range,
                 attack_power, price,    is_aerial, state} {}

  [[nodiscard]] static Defender *construct(DefenderType type, Position p);

  [[nodiscard]] std::optional<size_t>
  get_nearest_attacker_index(const std::vector<Attacker *> &attackers) const;
};
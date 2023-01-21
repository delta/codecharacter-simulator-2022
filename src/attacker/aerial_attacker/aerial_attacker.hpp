#pragma once

#include "actor/actor.hpp"
#include "attacker/attacker.hpp"
#include "utils/attributes.hpp"
#include "utils/position.hpp"

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

class Defender;

class AerialAttacker : public Attacker {

public:
  AerialAttacker(AttackerType type, Position position, unsigned hp,
                 unsigned range, unsigned speed, unsigned attack_power,
                 unsigned price, bool is_aerial, State state = State::SPAWNED)
      : Attacker{type,         position, hp,        range, speed,
                 attack_power, price,    is_aerial, state} {}

  [[nodiscard]] static Attacker *construct(AttackerType type, Position p);

  [[nodiscard]] std::optional<size_t>
  get_nearest_defender_index(const std::vector<Defender *> &defenders) const;
};

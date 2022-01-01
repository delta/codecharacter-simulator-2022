#pragma once

#include "attacker/attacker.hpp"
#include "defender/defender.hpp"
#include "utils/position.hpp"

#include <memory>
#include <vector>

class Game {

public:
  Game(std::vector<Attacker> attackers, std::vector<Defender> defenses);

  [[nodiscard]] const std::vector<Attacker> &get_attackers() const;

  [[nodiscard]] const std::vector<Defender> &get_defenders() const;

  Game simulate(
      const std::vector<std::pair<Position, AttackerType>> &spawn_postions);

private:
  std::vector<Attacker> _attackers;
  std::vector<Defender> _defenses;

  [[nodiscard]] size_t get_defender_from_position(Position p) const;
  [[nodiscard]] size_t get_attacker_from_position(Position p) const;
};

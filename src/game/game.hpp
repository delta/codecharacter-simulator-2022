#pragma once

#include "attacker/attacker.hpp"
#include "defender/defender.hpp"
#include "utils/position.hpp"

#include <memory>
#include <vector>

class Game {

public:
  Game(std::vector<Attacker> attackers, std::vector<Defender> defenses,
       unsigned coins);

  [[nodiscard]] const std::vector<Attacker> &get_attackers() const;

  [[nodiscard]] const std::vector<Defender> &get_defenders() const;

  Game simulate(
      const std::vector<std::pair<Position, AttackerType>> &spawn_postions);

  [[nodiscard]] unsigned get_coins() const;

private:
  std::vector<Attacker> _attackers;
  std::vector<Defender> _defenses;
  unsigned _coins;

};

#pragma once

#include "attacker/attacker.hpp"
#include "defender/defender.hpp"
#include "utils/position.hpp"

#include <memory>
#include <vector>

class Game {

public:
  Game(std::vector<Attacker> attackers, std::vector<Defender> defenders,
       unsigned coins);

  using defender_id = size_t;
  using attacker_id = size_t;
  using index_t = size_t;

  [[nodiscard]] const std::vector<Attacker> &get_attackers() const;

  [[nodiscard]] const std::vector<Defender> &get_defenders() const;

  [[nodiscard]] Game simulate(
      const std::unordered_map<attacker_id, defender_id> &player_set_targets,
      const std::vector<std::pair<Position, AttackerType>> &spawn_positions)
      const;

  [[nodiscard]] unsigned get_coins() const;

private:
  std::optional<index_t> get_attacker_index_by_id(attacker_id id) const;
  std::optional<index_t> get_defender_index_by_id(defender_id id) const;

  std::unordered_map<attacker_id, size_t> _attacker_id_to_index;
  std::unordered_map<defender_id, size_t> _defender_id_to_index;
  std::vector<Attacker> _attackers;
  std::vector<Defender> _defenders;
  unsigned _coins;
};

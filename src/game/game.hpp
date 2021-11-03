#include "../attack/troop.hpp"
#include "../defense/defense.hpp"
#include "../utils/position.hpp"

#include <memory>
#include <vector>

enum class TroopType { X, Y };

class Game {

public:
  template <typename T> using vec_of_ptr = std::vector<std::unique_ptr<T>>;

  Game(vec_of_ptr<Troop> troops, vec_of_ptr<Defense> defenses);

  Game
  simulate(const std::vector<std::pair<Position, TroopType>> &spawn_postions);

  const vec_of_ptr<Troop> &get_troops() const;

  const vec_of_ptr<Defense> &get_defenses() const;

private:
  vec_of_ptr<Troop> _troops;
  vec_of_ptr<Defense> _defenses;
};

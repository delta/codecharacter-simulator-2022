#pragma once

#include "defender/defender.hpp"

#include <iostream>
#include <vector>

class Map {
public:
  Map(std::vector<std::vector<int>> map_as_grid);

  static Map get(std::istream &stream);

  [[nodiscard]] std::vector<Defender> spawn_defenders() const;

  static inline size_t no_of_rows = 64;
  static inline size_t no_of_cols = 64;

private:
  std::vector<std::vector<int>> _grid;
};

#pragma once

#include "defender/defender.hpp"

#include <iostream>
#include <vector>

class Map {
public:
  Map(size_t no_of_rows, size_t no_of_cols,
      std::vector<std::vector<int>> map_as_grid);

  static Map get(std::istream &stream);

  [[nodiscard]] std::vector<Defender> spawn_defenders() const;

private:
  size_t _no_of_rows;
  size_t _no_of_cols;
  std::vector<std::vector<int>> _grid;
};

#include "utils/game_map.hpp"

Map::Map(std::vector<std::vector<int>> map_as_grid)
    : _grid(move(map_as_grid)) {}

Map Map::get(std::istream &stream) {
  size_t m = 0;
  size_t n = 0;
  stream >> m;
  stream >> n;

  Map::no_of_rows = m;
  Map::no_of_cols = n;

  std::vector<std::vector<int>> grid(m, std::vector<int>(n, 0));
  for (auto &row : grid) {
    for (auto &cell : row) {
      stream >> cell;
    }
  }
  return {move(grid)};
}

[[nodiscard]] std::vector<Defender> Map::spawn_defenders() const {
  std::vector<Defender> defenders;
  for (size_t x = 0; x < this->no_of_cols; x++) {
    for (size_t y = 0; y < this->no_of_rows; y++) {
      if (this->_grid[y][x] > 0) {
        defenders.push_back(Defender::construct(
            static_cast<DefenderType>(this->_grid[y][x]),
            Position(static_cast<int>(x), static_cast<int>(y))));
      }
    }
  }

  return defenders;
}

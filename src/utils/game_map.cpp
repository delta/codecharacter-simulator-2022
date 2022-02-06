#include "utils/game_map.hpp"

Map::Map(size_t no_of_rows, size_t no_of_cols,
         std::vector<std::vector<int>> map_as_grid)
    : _no_of_rows(no_of_rows), _no_of_cols(no_of_cols),
      _grid(move(map_as_grid)) {}

Map Map::get(std::istream &stream) {
  size_t m = 0;
  size_t n = 0;
  stream >> m;
  stream >> n;
  std::vector<std::vector<int>> grid(m, std::vector<int>(n, 0));
  for (auto &row : grid) {
    for (auto &cell : row) {
      stream >> cell;
    }
  }
  return {m, n, move(grid)};
}

[[nodiscard]] std::vector<Defender> Map::spawn_defenders() const {
  std::vector<Defender> defenders;
  for (size_t i = 0; i < this->_no_of_rows; i++) {
    for (size_t j = 0; j < this->_no_of_cols; j++) {
      if (this->_grid[i][j] > 0) {
        defenders.push_back(Defender::construct(
            static_cast<DefenderType>(this->_grid[i][j]),
            Position(static_cast<int>(i), static_cast<int>(j))));
      }
    }
  }
  return defenders;
}

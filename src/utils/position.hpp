#pragma once

#include <compare>

class Position {
private:
  int _x;
  int _y;

public:
  /*
   *  handle out of map positions
   */
  Position(int x, int y);
  [[nodiscard]] int get_x() const;
  [[nodiscard]] int get_y() const;
  [[nodiscard]] double distance_to(Position other) const;
  auto operator<=>(const Position &other) const = default;
  static bool is_valid(int x, int y);
};

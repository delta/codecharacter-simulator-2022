#pragma once

class Position {
private:
  int _x;
  int _y;

public:
  /*
   *  handle out of map positions
   */
  Position(int x, int y);
  int get_x() const;
  int get_y() const;
  float distance_to(Position other) const;
  static bool is_valid(int x, int y);
};

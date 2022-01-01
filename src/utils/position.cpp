#include "utils/position.hpp"

#include <cmath>

Position::Position(int x, int y) : _x(x), _y(y) {}

int Position::get_x() const { return this->_x; }

int Position::get_y() const { return this->_y; }

double Position::distance_to(Position other) const {
  auto delta_x = other.get_x() - this->get_x();
  auto delta_y = other.get_y() - this->get_y();
  return sqrt((double)(delta_x * delta_x + delta_y * delta_y));
}

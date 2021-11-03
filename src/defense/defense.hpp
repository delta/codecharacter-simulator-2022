#pragma once

#include "../utils/position.hpp"

class Troop;

class Defense {
public:
  enum class Status { ALIVE, DEAD };

  Defense(Position position, unsigned hp, unsigned range, unsigned attack_power,
          unsigned price, Status status = Status::ALIVE);

  bool is_in_range(const Troop &troop) const;

private:
  Position _position;
  unsigned _hp;
  Status _status;
  const unsigned _range;
  const unsigned _attack_power;
  const unsigned _price;
};

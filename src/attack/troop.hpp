#pragma once

#include "../utils/position.hpp"

#include <memory>
#include <vector>

class Defense;

class Troop {
public:
  enum class Status { ALIVE, DEAD };
  Troop(Position position, unsigned hp, unsigned range, unsigned speed,
        unsigned attack_power, unsigned price, Status status = Status::ALIVE);

  bool is_in_range(const Defense &defender) const;

  Position get_nearest_defense_postion(
      const std::vector<std::unique_ptr<Defense>> &defenses) const;

  void attack(std::unique_ptr<Defense> &defense);

  void move(Position position);

private:
  Position _position;
  unsigned _hp;
  Status _status;
  const unsigned _range;
  const unsigned _speed;
  const unsigned _attack_power;
  const unsigned _price;
};

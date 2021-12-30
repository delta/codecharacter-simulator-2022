#pragma once

#include "../utils/position.hpp"

#include <memory>
#include <vector>

class Attacker;

class Actor {
public:
  Actor(Position position, unsigned hp, unsigned range, unsigned attack_power,
        unsigned price);

  virtual ~Actor() = default;

  [[nodiscard]] bool is_in_range(const Actor &actor) const;

  void attack(Actor &opponent) const;

  void take_damage(unsigned damage);

  [[nodiscard]] unsigned get_hp() const;

  [[nodiscard]] unsigned get_attack_power() const;

  [[nodiscard]] unsigned get_price() const;

  [[nodiscard]] unsigned get_range() const;

  [[nodiscard]] Position get_position() const;

  virtual void update_state() = 0;

private:
  Position _position;
  unsigned _hp;
  const unsigned _range;
  const unsigned _attack_power;
  const unsigned _price;
};

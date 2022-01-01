#pragma once

#include "../utils/position.hpp"

#include <memory>
#include <vector>

class Actor {
public:
  Actor(size_t id, Position position, unsigned hp, unsigned range,
        unsigned attack_power, unsigned price);

  virtual ~Actor() = default;

  [[nodiscard]] bool is_in_range(const Actor &actor) const;

  void attack(Actor &opponent) const;

  void take_damage(unsigned damage);

  [[nodiscard]] size_t get_id() const;

  [[nodiscard]] unsigned get_hp() const;

  [[nodiscard]] unsigned get_attack_power() const;

  [[nodiscard]] unsigned get_price() const;

  [[nodiscard]] unsigned get_range() const;

  [[nodiscard]] Position get_position() const;

  virtual void update_state() = 0;

private:
  size_t _id;
  Position _position;
  unsigned _hp;
  const unsigned _range;
  const unsigned _attack_power;
  const unsigned _price;
};

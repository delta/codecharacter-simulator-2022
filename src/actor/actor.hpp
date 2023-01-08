#pragma once

#include "utils/position.hpp"

#include <memory>
#include <vector>

class Actor {
public:
  Actor(size_t id, Position position, unsigned hp, unsigned range,
        unsigned attack_power, unsigned price, bool is_aerial);

  virtual ~Actor() = default;

  Actor(const Actor &other) = default;
  Actor &operator=(const Actor &other) = default;
  Actor(Actor &&other) = default;
  Actor &operator=(Actor &&other) = default;

  bool operator==(const Actor &other) const;

  [[nodiscard]] bool is_in_range(const Actor &actor) const;

  virtual void attack(Actor &opponent) const = 0;

  void take_damage(unsigned damage);

  [[nodiscard]] size_t get_id() const;

  [[nodiscard]] unsigned get_hp() const;

  [[nodiscard]] unsigned get_attack_power() const;

  [[nodiscard]] unsigned get_price() const;

  [[nodiscard]] unsigned get_range() const;

  [[nodiscard]] Position get_position() const;

  bool is_aerial_type() const;

  virtual void update_state() = 0;

protected:
  size_t _id;
  Position _position;
  unsigned _hp;

private:
  unsigned _range;
  unsigned _attack_power;
  unsigned _price;
  bool _is_aerial;
};

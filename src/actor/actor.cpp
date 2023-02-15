#include "actor/actor.hpp"
#include "logger/logger.hpp"

#include <cmath>

Actor::Actor(size_t id, Position position, unsigned hp, unsigned range,
             unsigned attack_power, unsigned price, bool is_aerial)
    : _id(id), _position(position), _hp(hp), _range(range),
      _attack_power(attack_power), _price(price), _is_aerial(is_aerial) {}

bool Actor::operator==(const Actor &other) const {
  return this->_attack_power == other._attack_power && this->_hp == other._hp &&
         this->_position == other._position && this->_price == other._price &&
         this->_range == other._range && this->_is_aerial == other._is_aerial;
}

void Actor::take_damage(unsigned damage) {
  if (damage <= this->_hp) {
    this->_hp -= damage;
  } else {
    this->_hp = 0;
  }
}

size_t Actor::get_id() const { return this->_id; }

unsigned Actor::get_hp() const { return this->_hp; }

unsigned Actor::get_attack_power() const { return this->_attack_power; }

unsigned Actor::get_price() const { return this->_price; }

unsigned Actor::get_range() const { return this->_range; }

Position Actor::get_position() const { return this->_position; }

bool Actor::is_aerial_type() const { return this->_is_aerial; }

bool Actor::is_in_range(const Actor &actor) const {
  return round(this->get_position().distance_to(actor.get_position())) <=
         (double)this->_range;
}

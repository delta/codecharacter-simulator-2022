#include "actor.hpp"
#include "../logger/logger.hpp"

Actor::Actor(size_t id, Position position, unsigned hp, unsigned range,
             unsigned attack_power, unsigned price)
    : _id(id), _position(position), _hp(hp), _range(range),
      _attack_power(attack_power), _price(price) {}

void Actor::attack(Actor &opponent) const {
  opponent.take_damage(this->get_attack_power());
  Logger::log_shoot(this->_id, opponent._id, opponent.get_hp());
}

void Actor::take_damage(unsigned damage) {
  this->_hp -= std::min(damage, this->_hp);
}

unsigned Actor::get_hp() const { return this->_hp; }

unsigned Actor::get_attack_power() const { return this->_attack_power; }

unsigned Actor::get_price() const { return this->_price; }

unsigned Actor::get_range() const { return this->_range; }

Position Actor::get_position() const { return this->_position; }

bool Actor::is_in_range(const Actor &actor) const {
  return this->get_position().distance_to(actor.get_position()) <=
         (double)this->_range;
}

#include "utils/attributes.hpp"

Attributes::Attributes(unsigned hp, unsigned range, unsigned attack_power,
                       unsigned speed, unsigned price)
    : _hp(hp), _range(range), _attack_power(attack_power), _speed(speed),
      _price(price) {}

Attributes::Attributes()
    : _hp(0), _range(0), _attack_power(0), _speed(0), _price(0) {}

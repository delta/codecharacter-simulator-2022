#include "utils/attributes.hpp"

Attributes::Attributes(unsigned hp, unsigned range, unsigned attack_power,
                       unsigned speed, unsigned price)
    : hp(hp), range(range), attack_power(attack_power), speed(speed),
      price(price) {}

Attributes::Attributes()
    : hp(0), range(0), attack_power(0), speed(0), price(0) {}

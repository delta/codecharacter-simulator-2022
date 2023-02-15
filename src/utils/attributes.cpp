#include "utils/attributes.hpp"

Attributes::Attributes(unsigned hp, unsigned range, unsigned attack_power,
                       unsigned speed, unsigned price, bool is_aerial)
    : hp(hp), range(range), attack_power(attack_power), speed(speed),
      price(price), is_aerial(is_aerial) {}

Attributes::Attributes()
    : hp(0), range(0), attack_power(0), speed(0), price(0), is_aerial(false) {}

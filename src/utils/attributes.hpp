#pragma once

struct Attributes {
  const unsigned _hp;
  const unsigned _range;
  const unsigned _attack_power;
  const unsigned _speed;
  const unsigned _price;
  Attributes(unsigned hp, unsigned range, unsigned attack_power, unsigned speed,
             unsigned price);

  /**
   * A default constructor is to be added for this, solely for the reason of
   * this being stored as a value in unordered_map. unordered_map's operator[]
   * requires its value type to be default constructible and hence a default
   * constructor is to be defined. If I didnt use the operator[] of
   * unordered_map(which is being used however) , I wouldnt need to do this
   */
  Attributes();
};

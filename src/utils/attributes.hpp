#pragma once

struct Attributes {
  const unsigned hp;
  const unsigned range;
  const unsigned attack_power;
  const unsigned speed;
  const unsigned price;
  const bool is_aerial;
  Attributes(unsigned hp, unsigned range, unsigned attack_power, unsigned speed,
             unsigned price, bool is_aerial);

  /**
   * A default constructor is to be added for this, solely for the reason of
   * this being stored as a value in unordered_map. unordered_map's operator[]
   * requires its value type to be default constructible and hence a default
   * constructor is to be defined. If I didnt use the operator[] of
   * unordered_map(which is being used however) , I wouldnt need to do this
   */
  Attributes();
};

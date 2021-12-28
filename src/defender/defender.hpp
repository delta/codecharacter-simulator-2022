#pragma once

#include "../actor/actor.hpp"
#include "../utils/attributes.hpp"
#include "../utils/position.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

enum class DefenderType { X, Y };

class Attacker;

class Defender : public Actor {
public:
  static std::unordered_map<DefenderType, Attributes> attribute_dictionary;

  enum class State { IDLE, ATTACKING, DEAD };

  Defender(Position position, unsigned hp, unsigned range,
           unsigned attack_power, unsigned price, State status = State::IDLE);

  static Defender construct(DefenderType type, Position p);

  [[nodiscard]] Position
  get_nearest_attacker_postion(const std::vector<Attacker> &attackers) const;

  void update_state() final;

  void set_state(State s);

private:
  State _state;
};

#pragma once

#include "../actor/actor.hpp"
#include "../utils/attributes.hpp"
#include "../utils/position.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

class Defender;

enum class AttackerType { X, Y };

class Attacker : public Actor {
public:
  static std::unordered_map<AttackerType, Attributes> attribute_dictionary;

  enum class State { SPAWNED, ATTACKING, MOVING, DEAD };
  Attacker(Position position, unsigned hp, unsigned range, unsigned speed,
           unsigned attack_power, unsigned price,
           State status = State::SPAWNED);

  static Attacker construct(AttackerType type, Position p);

  [[nodiscard]] Position
  get_nearest_defense_postion(const std::vector<Defender> &defenses) const;

  void move(Position position);

  [[nodiscard]] bool is_destination_set() const;

  void clear_destination();

  void set_destination(Position p);
  Position get_destination();

  void update_state() final;

private:
  void set_state(State s);

  State _state;
  unsigned _speed;
  bool _is_dest_set;
  Position _destination;
};

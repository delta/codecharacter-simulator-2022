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
  static inline std::unordered_map<AttackerType, Attributes>
      attribute_dictionary;

  enum class State { SPAWNED, ATTACKING, MOVING, DEAD };
  Attacker(AttackerType type, Position position, unsigned hp, unsigned range,
           unsigned speed, unsigned attack_power, unsigned price,
           State state = State::SPAWNED)
      : Actor{position, hp, range, attack_power, price}, _type(type),
        _state(state), _speed(speed), _destination{0, 0} {}

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

  AttackerType _type;
  State _state;
  unsigned _speed;
  bool _is_dest_set;
  Position _destination;
};

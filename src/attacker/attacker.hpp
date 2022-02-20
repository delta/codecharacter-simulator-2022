#pragma once

#include "actor/actor.hpp"
#include "utils/attributes.hpp"
#include "utils/position.hpp"

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

class Defender;

enum class AttackerType {
  X = 1,
  Y,
};

class Attacker : public Actor {

public:
  static inline std::unordered_map<AttackerType, Attributes>
      attribute_dictionary;

  enum class State { SPAWNED, ATTACKING, MOVING, DEAD };

  Attacker(AttackerType type, Position position, unsigned hp, unsigned range,
           unsigned speed, unsigned attack_power, unsigned price,
           State state = State::SPAWNED)
      : Actor{_id_counter++, position, hp, range, attack_power, price},
        _type(type), _state(state), _speed(speed), _is_dest_set(false),
        _destination{0, 0}, _is_target_set_by_player{0}, _target_id{0} {}

  [[nodiscard]] static Attacker construct(AttackerType type, Position p);

  [[nodiscard]] std::optional<size_t>
  get_nearest_defender_index(const std::vector<Defender> &defenders) const;

  void move(Position position);

  void attack(Actor &opponent) const override;

  [[nodiscard]] bool is_destination_set() const;

  void clear_destination();

  void set_destination(Position p);

  [[nodiscard]] Position get_destination() const;

  [[nodiscard]] bool is_target_set_by_player() const;

  [[nodiscard]] size_t get_target_id() const;

  void set_target(size_t defender_id);

  void clear_target();

  void update_state() final;

  [[nodiscard]] AttackerType get_type() const;

  [[nodiscard]] State get_state() const;

private:
  void set_state(State s);

  static inline size_t _id_counter = 0;
  AttackerType _type;
  State _state;
  unsigned _speed;
  bool _is_dest_set;
  Position _destination;
  bool _is_target_set_by_player;
  size_t _target_id;
};

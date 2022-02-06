#pragma once

#include "actor/actor.hpp"
#include "utils/attributes.hpp"
#include "utils/position.hpp"

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

enum class DefenderType {
  X = 1,
  Y,
};

class Attacker;

class Defender : public Actor {

public:
  static inline std::unordered_map<DefenderType, Attributes>
      attribute_dictionary;

  enum class State { IDLE, ATTACKING, DEAD };

  Defender(DefenderType type, Position position, unsigned hp, unsigned range,
           unsigned attack_power, unsigned price, State state = State::IDLE)
      : Actor{_id_counter++, position, hp, range, attack_power, price},
        _type(type), _state(state) {}

  [[nodiscard]] static Defender construct(DefenderType type, Position p);

  [[nodiscard]] std::optional<size_t>
  get_nearest_attacker_index(const std::vector<Attacker> &attackers) const;

  void update_state() final;

  void log_shoot(size_t actor_id, size_t opponent_id,
                 unsigned new_opponent_hp) const;

  void set_state(State s);

  [[nodiscard]] DefenderType get_type() const;

  [[nodiscard]] State get_state() const;

private:
  static inline size_t _id_counter;
  DefenderType _type;
  State _state;
};

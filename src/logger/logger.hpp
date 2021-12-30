#pragma once

#include <sstream>
#include <vector>

#include "../attacker/attacker.hpp"
#include "../defender/defender.hpp"

/*
 * A singleton class that handles logging
 */
class Logger {

public:
  static Logger &get() {
    static Logger logger;
    return logger;
  }
  Logger(Logger const &) = delete;
  Logger(Logger &&) = delete;
  Logger &operator=(Logger const &) = delete;
  Logger &operator=(Logger &&) = delete;

  static std::ostringstream &get_stream() { return get()._stream; }
  static void log_init(std::vector<Defender> &defenders);
  static void log_turn(unsigned turn);
  static void log_move(size_t attackerId, int x, int y);
  static void log_shoot(size_t attackerId, size_t defenderId,
                        unsigned targetHp);
  static void log_spawn(size_t attackerId, AttackerType type, int x, int y);
  static void log_dead(char actorType, size_t actorId);
  static void log_destruction(unsigned percent);
  static void log_coins(unsigned coins);
  static void log_end();
  [[nodiscard]] static std::string get_log();

private:
  std::ostringstream _stream;

  Logger() = default;
  ~Logger() = default;
};

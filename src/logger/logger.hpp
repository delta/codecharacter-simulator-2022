#pragma once

#include <algorithm>
#include <memory>
#include <sstream>
#include <vector>

#include "../defender/defender.hpp"

template <typename T> using vec_of_ptr = std::vector<std::unique_ptr<T>>;

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
  static void log_init(vec_of_ptr<Defender> &defenders);
  static void log_turn(int turn);
  static void log_move(int attackerId, int x, int y);
  static void log_shoot(int attackerId, size_t defenderId, unsigned targetHp);
  static void log_spawn(size_t defenderId, DefenderType type, int x, int y);
  static void log_dead(size_t defenderId);
  static void log_destruction(int percent);
  static void log_coins(int coins);
  static void log_end();
  [[nodiscard]] static std::string get_log();

private:
  std::ostringstream _stream;

  Logger() = default;
  ~Logger() = default;
};

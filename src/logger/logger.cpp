#include <algorithm>
#include <sstream>
#include <vector>

#include "../attacker/attacker.hpp"
#include "../defender/defender.hpp"
#include "logger.hpp"

void Logger::log_init(std::vector<Defender> &defenders) {
  Logger::get_stream() << "INIT"
                       << "\n"
                       << "TOWERS"
                       << ", " << defenders.size();
  size_t defenderId = 0;
  std::ranges::for_each(defenders, [&](const auto &defender) {
    Logger::get_stream() << ", "
                         << "(" << defenderId++ << ", "
                         << (int)defender.get_type() << ", "
                         << defender.get_position().get_x() << ", "
                         << defender.get_position().get_y() << ", "
                         << defender.get_hp() << ")";
  });
  Logger::get_stream() << "\n";
}

void Logger::log_turn(unsigned turn) {
  Logger::get_stream() << "TURN"
                       << ", " << turn << "\n";
}

void Logger::log_move(size_t attackerId, int x, int y) {
  Logger::get_stream() << "MOVE"
                       << ", " << attackerId << ", " << x << ", " << y << "\n";
}

void Logger::log_shoot(size_t attackerId, size_t defenderId,
                       unsigned targetHp) {
  Logger::get_stream() << "SHOOT"
                       << ", " << attackerId << ", " << defenderId << ", "
                       << targetHp << "\n";
}

void Logger::log_spawn(size_t attackerId, AttackerType type, int x, int y) {
  Logger::get_stream() << "SPAWN"
                       << ", " << attackerId << ", " << (int)type << ", " << x
                       << ", " << y << "\n";
}

void Logger::log_dead(char actorType, size_t actorId) {
  Logger::get_stream() << "DEAD"
                       << ", " << actorType << ", " << actorId << "\n";
}

void Logger::log_destruction(unsigned percent) {
  Logger::get_stream() << "DESTRUCTION"
                       << ", " << percent << "%"
                       << "\n";
}

void Logger::log_coins(unsigned coins) {
  Logger::get_stream() << "COINS"
                       << ", " << coins << "\n";
}

void Logger::log_end() {
  Logger::get_stream() << "END"
                       << "\n";
}

[[nodiscard]] std::string Logger::get_log() {
  return Logger::get_stream().str();
}

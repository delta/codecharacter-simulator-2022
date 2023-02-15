#include "logger/logger.hpp"
#include "attacker/attacker.hpp"
#include "defender/defender.hpp"

#include <algorithm>
#include <sstream>
#include <vector>

void Logger::log_init(std::vector<Defender *> &defenders) {
  Logger::get_stream() << "INIT"
                       << "\n"
                       << "TOWERS"
                       << ", " << defenders.size();
  size_t defender_id = 0;
  std::ranges::for_each(defenders, [&](const Defender *defender) {
    Logger::get_stream() << ", "
                         << "(" << defender_id++ << ", "
                         << (int)defender->get_type() << ", "
                         << defender->get_position().get_x() << ", "
                         << defender->get_position().get_y() << ", "
                         << defender->get_hp() << ")";
  });
  Logger::get_stream() << "\n";
}

void Logger::log_turn(unsigned turn) {
  Logger::get_stream() << "TURN"
                       << ", " << turn << "\n";
}

void Logger::log_move(size_t attacker_id, int x, int y) {
  Logger::get_stream() << "MOVE"
                       << ", " << attacker_id << ", " << x << ", " << y << "\n";
}

void Logger::log_shoot(char opponent_actor_type, size_t attacker_id,
                       size_t defender_id, unsigned target_hp) {
  Logger::get_stream() << "SHOOT"
                       << ", " << opponent_actor_type << ", " << attacker_id
                       << ", " << defender_id << ", " << target_hp << "\n";
}

void Logger::log_spawn(size_t attacker_id, AttackerType type, int x, int y) {
  Logger::get_stream() << "SPAWN"
                       << ", " << attacker_id << ", " << (int)type << ", " << x
                       << ", " << y << "\n";
}

void Logger::log_dead(char actor_type, size_t actor_id) {
  Logger::get_stream() << "DEAD"
                       << ", " << actor_type << ", " << actor_id << "\n";
}

void Logger::log_destruction(double percent) {
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

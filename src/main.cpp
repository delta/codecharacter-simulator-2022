#include <iostream>

#include "defender/defender.hpp"
#include "logger/logger.hpp"

template <typename T> using vec_of_ptr = std::vector<std::unique_ptr<T>>;

int main() {
  /*
   * 1) we'll need to get a few things as input for the first time
   *		- Defense map (will be given by driver)
   *		- Troop types and their starting attributes  (will be given by
   *					driver)
   *		- Defense types and their starting attributes (will be given by
   *					driver)
   *		- No. of turns also from driver because why not
   *
   * 2) Print out the state of the game
   * 3) Attacker will read this and make their move
   * 4) Attacker prints spawn position and we'll read it here and process it
   * 5) We'll run simulations
   * 6) Go to 2) until end of game?
   */
  std::cout << "CODE CHARACTER 2022!!!!!!\n";
  auto defenders = vec_of_ptr<Defender>();
  Logger::log_init(defenders);
  Logger::log_turn(1);
  Logger::log_move(0, 0, 0);
  Logger::log_shoot(0, 0, 0);
  Logger::log_spawn(0, DefenderType::X, 0, 0);
  Logger::log_dead(0);
  Logger::log_destruction(0);
  Logger::log_coins(0);
  std::cout << Logger::get_log();
}

#include <algorithm>
#include <iostream>
#include <numeric>
#ifndef __clang__ // Ranges is buggy in clang
#include <ranges>
#endif
#include "attacker/attacker.hpp"
#include "defender/defender.hpp"
#include "game/game.hpp"
#include "logger/logger.hpp"

int main() {
  size_t map_x, map_y;
  std::cin >> map_x >> map_y;

  unsigned turns, coins;
  std::cin >> turns >> coins;

  unsigned n_attacker_types;
  std::cin >> n_attacker_types;
#ifdef __clang__ // https://gcc.gnu.org/bugzilla/show_bug.cgi?id=97120
  for (unsigned attacker_type_id = 0; attacker_type_id < n_attacker_types;
       ++attacker_type_id) {
#else
  for (unsigned attacker_type_id : std::views::iota(0u, n_attacker_types)) {
#endif
    unsigned hp, range, attack_power, speed, price;
    std::cin >> hp >> range >> attack_power >> speed >> price;
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType(attacker_type_id),
                       Attributes(hp, range, attack_power, speed, price)));
  }

  unsigned n_defender_types;
  std::cin >> n_defender_types;
#ifdef __clang__
  for (unsigned defender_type_id = 0; defender_type_id < n_defender_types;
       ++defender_type_id) {
#else
  for (unsigned defender_type_id : std::views::iota(0u, n_defender_types)) {
#endif
    unsigned hp, range, attack_power, speed, price;
    std::cin >> hp >> range >> attack_power >> speed >> price;
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType(defender_type_id),
                       Attributes(hp, range, attack_power, speed, price)));
  }

  auto defenders = std::vector<Defender>();
  size_t n_defenders;
  std::cin >> n_defenders;
#ifdef __clang__
  for (size_t defender_id = 0; defender_id < n_defenders; ++defender_id) {
#else
  for (size_t defender_id : std::views::iota(0u, n_defenders)) {
#endif
    unsigned type_id, x, y;
    std::cin >> type_id >> x >> y;
    defenders.emplace_back(Defender::construct(
        defender_id, DefenderType(type_id), Position(x, y)));
  }

  auto initial_hp = std::accumulate(defenders.begin(), defenders.end(), 0,
                                    [](unsigned acc, const Defender &defender) {
                                      return acc + defender.get_hp();
                                    });

  Logger::log_init(defenders);

  Game game({}, defenders, coins);

  size_t attacker_id = 0;
#ifdef __clang__
  for (size_t turn = 0; turn < turns; ++turn) {
#else
  for (size_t turn : std::views::iota(0u, turns)) {
#endif
    unsigned n_attackers;
    std::cin >> n_attackers;
    Logger::log_turn(turn);

    auto spawn_positions =
        std::vector<std::tuple<size_t, Position, AttackerType>>();
    while (n_attackers-- > 0) {
      unsigned type_id, x, y;
      std::cin >> type_id >> x >> y;
      Logger::log_spawn(attacker_id, AttackerType(type_id), x, y);
      spawn_positions.emplace_back(
          std::make_tuple(attacker_id, Position(x, y), AttackerType(type_id)));
      game = game.simulate(spawn_positions);

      auto active_attackers = game.get_attackers();
      std::cout << active_attackers.size() << "\n";
      std::ranges::for_each(active_attackers, [](const Attacker &attacker) {
        std::cout << attacker.get_position().get_x() << " "
                  << attacker.get_position().get_y() << " "
                  << (int)attacker.get_type() << " " << attacker.get_hp()
                  << "\n";
      });

      auto active_defenders = game.get_defenders();
      std::cout << active_defenders.size() << "\n";
      std::ranges::for_each(active_defenders, [](const Defender &defender) {
        std::cout << defender.get_position().get_x() << " "
                  << defender.get_position().get_y() << " "
                  << (int)defender.get_type() << " " << defender.get_hp()
                  << "\n";
      });

      std::cout << game.get_coins() << "\n";

      auto current_hp =
          std::accumulate(active_defenders.begin(), active_defenders.end(), 0,
                          [](unsigned acc, const Defender &defender) {
                            return acc + defender.get_hp();
                          });

      Logger::log_destruction(100 -
                              (initial_hp - current_hp) * 100 / initial_hp);
      Logger::log_coins(game.get_coins());
      ++attacker_id;
    }
  }
  Logger::log_end();
  std::cout << Logger::get_log();
}

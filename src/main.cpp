#include <algorithm>
#include <iostream>
#include <numeric>

#include "attacker/attacker.hpp"
#include "defender/defender.hpp"
#include "game/game.hpp"
#include "logger/logger.hpp"

int main() {
  size_t mapX, mapY;
  std::cin >> mapX >> mapY;

  unsigned turns, coins;
  std::cin >> turns >> coins;

  unsigned nAttackerTypes;
  std::cin >> nAttackerTypes;
  for (unsigned i = 0; i < nAttackerTypes; ++i) {
    unsigned hp, range, attack_power, speed, price;
    std::cin >> hp >> range >> attack_power >> speed >> price;
    Attacker::attribute_dictionary.insert(std::make_pair(
        AttackerType(i), Attributes(hp, range, attack_power, speed, price)));
  }

  unsigned nDefenderTypes;
  std::cin >> nDefenderTypes;
  for (unsigned i = 0; i < nDefenderTypes; ++i) {
    unsigned hp, range, attack_power, speed, price;
    std::cin >> hp >> range >> attack_power >> speed >> price;
    Defender::attribute_dictionary.insert(std::make_pair(
        DefenderType(i), Attributes(hp, range, attack_power, speed, price)));
  }

  auto defenders = std::vector<Defender>();
  unsigned nDefenders;
  std::cin >> nDefenders;
  for (unsigned i = 0; i < nDefenders; ++i) {
    unsigned typeId, x, y;
    std::cin >> typeId >> x >> y;
    defenders.emplace_back(
        Defender::construct(i, DefenderType(typeId), Position(x, y)));
  }

  auto initialHp = std::accumulate(defenders.begin(), defenders.end(), 0,
                                   [](unsigned acc, const Defender &defender) {
                                     return acc + defender.get_hp();
                                   });

  Logger::log_init(defenders);

  Game game({}, defenders, coins);

  size_t attackerId = 0;
  for (unsigned turn = 0; turn < turns; ++turn) {
    unsigned nAttackers;
    std::cin >> nAttackers;
    Logger::log_turn(turn);

    auto spawn_positions =
        std::vector<std::tuple<size_t, Position, AttackerType>>();
    for (unsigned i = 0; i < nAttackers; ++i) {
      unsigned typeId, x, y;
      std::cin >> typeId >> x >> y;
      Logger::log_spawn(attackerId, AttackerType(typeId), x, y);
      spawn_positions.emplace_back(
          std::make_tuple(attackerId, Position(x, y), AttackerType(typeId)));
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

      auto currentHp =
          std::accumulate(active_defenders.begin(), active_defenders.end(), 0,
                          [](unsigned acc, const Defender &defender) {
                            return acc + defender.get_hp();
                          });

      Logger::log_destruction(100 - (initialHp - currentHp) * 100 / initialHp);
      Logger::log_coins(game.get_coins());
      ++attackerId;
    }
  }
  Logger::log_end();
  std::cout << Logger::get_log();
}

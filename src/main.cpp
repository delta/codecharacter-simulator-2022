#include <algorithm>
#include <iostream>
#include <numeric>

#include "defender/defender.hpp"
#include "game/game.hpp"
#include "logger/logger.hpp"
#include "utils/game_map.hpp"

int main() {
  unsigned turns, coins;
  std::cin >> turns >> coins;

  unsigned n_attacker_types;
  std::cin >> n_attacker_types;
  for (unsigned attacker_type_id = 1; attacker_type_id <= n_attacker_types;
       ++attacker_type_id) {
    unsigned hp, range, attack_power, speed, price;
    std::cin >> hp >> range >> attack_power >> speed >> price;
    Attacker::attribute_dictionary.insert(
        std::make_pair(AttackerType(attacker_type_id),
                       Attributes(hp, range, attack_power, speed, price)));
  }

  unsigned n_defender_types;
  std::cin >> n_defender_types;
  for (unsigned defender_type_id = 1; defender_type_id <= n_defender_types;
       ++defender_type_id) {
    unsigned hp, range, attack_power, speed, price;
    std::cin >> hp >> range >> attack_power >> speed >> price;
    Defender::attribute_dictionary.insert(
        std::make_pair(DefenderType(defender_type_id),
                       Attributes(hp, range, attack_power, speed, price)));
  }

  auto map = Map::get(std::cin);
  auto defenders = map.spawn_defenders();

  for (auto defender : defenders) {
    std::cerr << defender.get_hp() << std::endl;
  }

  auto initial_hp = std::accumulate(defenders.begin(), defenders.end(), 0,
                                    [](unsigned acc, const Defender &defender) {
                                      return acc + defender.get_hp();
                                    });

  Logger::log_init(defenders);

  Game game({}, defenders, coins);

  for (size_t turn = 0; turn < turns; ++turn) {
    Logger::log_turn(turn);

    unsigned n_attackers;
    std::cin >> n_attackers;

    auto spawn_positions = std::vector<std::pair<Position, AttackerType>>();
    while (n_attackers-- > 0) {
      unsigned type_id, x, y;
      std::cin >> type_id >> x >> y;
      spawn_positions.emplace_back(
          std::make_pair(Position(x, y), AttackerType(type_id)));
    }
    game = game.simulate(spawn_positions);

    auto active_attackers = game.get_attackers();
    std::cout << active_attackers.size() << "\n";
    std::ranges::for_each(active_attackers, [](const Attacker &attacker) {
      std::cout << attacker.get_position().get_x() << " "
                << attacker.get_position().get_y() << " "
                << (int)attacker.get_type() << " " << attacker.get_hp() << "\n";
    });

    auto active_defenders = game.get_defenders();
    std::cout << active_defenders.size() << "\n";
    std::ranges::for_each(active_defenders, [](const Defender &defender) {
      std::cout << defender.get_position().get_x() << " "
                << defender.get_position().get_y() << " "
                << (int)defender.get_type() << " " << defender.get_hp() << "\n";
    });

    std::cout << game.get_coins() << "\n";

    auto current_hp =
        std::accumulate(active_defenders.begin(), active_defenders.end(), 0,
                        [](unsigned acc, const Defender &defender) {
                          return acc + defender.get_hp();
                        });

    Logger::log_destruction((initial_hp - current_hp) * 100 / initial_hp);
    Logger::log_coins(game.get_coins());
  }
  Logger::log_end();

  // TODO: Figure out a way to save and extract logs from driver
  std::cerr << Logger::get_log();
}

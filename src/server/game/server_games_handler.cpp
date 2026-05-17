#include "server_games_handler.h"

GamesHandler::GamesHandler() {
  auto world_names = worlds_reader.get_world_names();
  uint8_t counter = 1;
  for (const auto &name : world_names) {
    worlds_map[counter] = name;
    counter++;
  }
}

void GamesHandler::delete_game(const uint8_t &game_id) {
  std::lock_guard<std::mutex> lck(m);
  auto dead = [game_id](Game *game) {
    if (game->compare_id(game_id)) {
      game->stop();
      game->join();
      delete game;
      return true;
    }
    return false;
  };

  games.erase(std::remove_if(games.begin(), games.end(), dead), games.end());
}

Game *GamesHandler::create_game(Player &player, const uint8_t &world_id, PlayerSender &sender,
                                ServerReceiver &receiver) {
  std::lock_guard<std::mutex> lck(m);
  std::string world_name = worlds_map[world_id];
  World world = worlds_reader.generate_world(world_name);

  Game *game = player.create_game(games_counter, world, games_config, sender, receiver);
  games_counter++;
  games.push_back(game);

  return game;
}

Game *GamesHandler::join_game(Player &player, const uint8_t &game_id, PlayerSender &sender,
                              ServerReceiver &receiver) {
  std::lock_guard<std::mutex> lck(m);
  Game *game = get_game(game_id);
  if (game == nullptr) {
    throw std::runtime_error("Error: No se encontró el juego al que se quiso unir.");
  }

  player.join_game(game, sender, receiver);

  return game;
}

Game *GamesHandler::start_game(Player &player, const uint8_t &game_id) {
  std::lock_guard<std::mutex> lck(m);
  Game *game = get_game(game_id);
  if (game == nullptr) {
    throw std::runtime_error("Error: No se encontró el juego que se quiso iniciar.");
  }

  player.start_game();
  return game;
}

Game *GamesHandler::get_game(const uint8_t &game_id) const {
  auto it = std::find_if(games.begin(), games.end(),
                         [game_id](Game *game) { return game->compare_id(game_id); });

  if (it != games.end()) {
    return *it;
  }
  return nullptr;
}

bool GamesHandler::game_exist(uint8_t game_id) {
  std::lock_guard<std::mutex> lck(m);

  auto exists = std::any_of(games.begin(), games.end(),
                            [game_id](Game *game) { return game->compare_id(game_id); });
  return exists;
}

void GamesHandler::reap_dead() {
  auto dead = [](Game *game) {
    if (game->is_dead()) {
      if (game->is_started()) {
        game->join();
      }
      delete game;
      return true;
    }
    return false;
  };

  games.erase(std::remove_if(games.begin(), games.end(), dead), games.end());
}

std::map<uint8_t, std::string> GamesHandler::get_worlds_map() const { return worlds_map; }

GamesHandler::~GamesHandler() {
  for (auto &current_game : games) {
    if (current_game->is_started()) {
      current_game->stop();
      current_game->join();
    }
    delete current_game;
  }
  games.clear();
}

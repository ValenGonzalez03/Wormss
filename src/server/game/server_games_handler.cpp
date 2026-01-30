#include "server_games_handler.h"

GamesHandler::GamesHandler() {
  auto world_names = worlds_reader.get_world_names();
  uint8_t counter = 1;
  for (const auto &name : world_names) {
    worlds_map[counter] = name;
    counter++;
  }
  // WorldsReader worlds_reader;
  // std::vector<std::shared_ptr<World>> worlds_aux;
  // worlds_aux = worlds_reader.read_yaml_files(std::filesystem::path(RESOURCES_PATH) / "Worlds");
  // int i = 0;
  // for (const auto& world : worlds_aux) {
  //   std::string world_stage = world->get_name();
  //   world_names.push_back(world_stage);
  //   worlds[i] = world;
  //   i++;
  // }
}

void GamesHandler::add_game(Game *game) { games.push_back(game); }

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

Game *GamesHandler::create_game(std::shared_ptr<Queue<GameState>> sender_queue, const uint8_t &player_id,
                                const uint8_t &world_id) {
  std::lock_guard<std::mutex> lck(m);
  std::string world_name = worlds_map[world_id];
  World world = worlds_reader.generate_world(world_name);

  Game *game = new Game(games_counter, games_config, world);
  games_counter++;
  add_game(game);

  game->add_player(sender_queue, player_id);
  return game;
}

Game *GamesHandler::join_game(std::shared_ptr<Queue<GameState>> sender_queue, const uint8_t &game_id,
                              const uint8_t &player_id) {
  std::lock_guard<std::mutex> lck(m);
  Game *game = get_game(game_id);
  if (game == nullptr) {
    return nullptr;  // Habria que tirar una excepcion.
  }

  if (!game->is_started()) {
    game->add_player(sender_queue, player_id);
    // Nuevamente, si la partida ya esta empezada habria que tirar una excepcion o avisar de alguna manera.
  }

  return game;
}

// World GamesHandler::select_world(int world_id, const uint8_t& game_id) {
//   std::string selected_world_name = world_names[world_id];
//   World selected_world;
//   for (const auto& world : worlds) {
//     std::string world_name = world->get_name();
//     if (world_name == selected_world_name) {
//       get_game(game_id)->set_world(*world);
//       return *world;
//     }
//   }
//   return selected_world;
// }

// World GamesHandler::get_game_world(const uint8_t& game_id) {
//   for (const auto& game : games) {
//     if (game_id == game->get_game_id()){
//       return game->get_world();
//     }
//   }
//   throw std::runtime_error("World not found");
// }

void GamesHandler::start_game(const uint8_t &game_id, const uint8_t &player_id) {
  std::lock_guard<std::mutex> lck(m);
  Game *game = get_game(game_id);
  if (game == nullptr) {
    return;
  }

  if (!game->is_started()) {
    game->turn_to_started();
    game->start();
  }
}

Game *GamesHandler::get_game(const uint8_t &game_id) {
  auto it = std::find_if(games.begin(), games.end(), [game_id](Game *game) { return game->compare_id(game_id); });

  if (it != games.end()) {
    return *it;
  }
  return nullptr;
}

bool GamesHandler::game_exist(uint8_t game_id) {
  std::lock_guard<std::mutex> lck(m);

  auto exists = std::any_of(games.begin(), games.end(), [game_id](Game *game) { return game->compare_id(game_id); });
  return exists;
  // for (auto &current_game : games) {
  //   if (current_game->compare_id(game_id)) {
  //     return true;
  //   }
  // }
  // return false;
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

// std::list<uint8_t> *GamesHandler::obtain_all_games_id() {
//   std::list<uint8_t> *games_id;
//   for (auto &current_game : games) {
//     games_id->push_back(current_game->get_game_id());
//   }
//   return games_id;
// }

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

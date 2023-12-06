#include "server_games_handler.h"

GamesHandler::GamesHandler() {
  WorldsReader worlds_reader;
  worlds = worlds_reader.read_yaml_files(std::filesystem::path(RESOURCES_PATH) / "Worlds");
  for (const auto& world : worlds) {
    std::string world_stage = world->get_name();
    world_names.push_back(world_stage);
  }
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

Queue<std::shared_ptr<RunnableCommandGame>> *
GamesHandler::create_game(std::shared_ptr<Queue<GameState>> sender_queue,
                          uint8_t& game_id, uint8_t &player_id, std::vector<std::string>& names) {
  std::lock_guard<std::mutex> lck(m);
  game_id = games_counter;
  Game *game = new Game(game_id, games_config);
  games_counter++;
  add_game(game);
  names = world_names;
  Queue<std::shared_ptr<RunnableCommandGame>> *commands_queue =
      game->add_player(sender_queue, player_id);
  return commands_queue;
}

Queue<std::shared_ptr<RunnableCommandGame>> *
GamesHandler::join_game(std::shared_ptr<Queue<GameState>> sender_queue,
                        const uint8_t &game_id, uint8_t &player_id) {
  std::lock_guard<std::mutex> lck(m);
  Queue<std::shared_ptr<RunnableCommandGame>> *commands_queue = nullptr;
  Game *game = get_game(game_id);
  if (game == nullptr) {
    return nullptr;
  }

  if (!game->is_started()) {
    commands_queue = game->add_player(sender_queue, player_id);
  }

  return commands_queue;
}

World GamesHandler::select_world(int world_id, const uint8_t& game_id) {
  std::string selected_world_name = world_names[world_id];
  World selected_world;
  for (const auto& world : worlds) {
    std::string world_name = world->get_name();
    if (world_name == selected_world_name) {
      get_game(game_id)->set_world(*world);
      return *world;
    }
  }
  return selected_world;
}

World& GamesHandler::select_world(const uint8_t& game_id) {
  for (const auto& game : games) {
    if (game_id == game->get_game_id()){
      return game->get_world();
    }
  }
  throw std::runtime_error("World not found");
}

void GamesHandler::start_game(const uint8_t &game_id,
                              const uint8_t &player_id) {
  if (player_id != 0)
    return;
  std::lock_guard<std::mutex> lck(m);
  Game *game = get_game(game_id);
  if (game == nullptr) {
    return;
  }

  if (!game->is_started()) {
    game->start();
    game->push_game_state();
  }
}

Game *GamesHandler::get_game(const uint8_t &game_id) {
  for (auto &current_game : games) {
    if (current_game->compare_id(game_id)) {
      return current_game;
    }
  }
  return nullptr;
}

bool GamesHandler::game_exist(uint8_t game_id) {
  std::lock_guard<std::mutex> lck(m);
  for (auto &current_game : games) {
    if (current_game->compare_id(game_id)) {
      return true;
    }
  }
  return false;
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

std::list<uint8_t> *GamesHandler::obtain_all_games_id() {
  std::list<uint8_t> *games_id;
  for (auto &current_game : games) {
    games_id->push_back(current_game->get_game_id());
  }
  return games_id;
}

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

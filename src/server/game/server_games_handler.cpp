#include "server_games_handler.h"

GamesHandler::GamesHandler() {
  auto world_names = worlds_reader.get_world_names();
  uint8_t counter = 1;
  for (const auto &name : world_names) {
    worlds_list[counter] = name;
    counter++;
  }
}

void GamesHandler::delete_game(const uint8_t &game_id) {
  std::lock_guard<std::mutex> lck(m);
  auto dead = [game_id](GameManager *game_man) {
    if (game_man->compare_id(game_id)) {
      game_man->stop();
      game_man->join();
      delete game_man;
      return true;
    }
    return false;
  };

  game_managers.erase(std::remove_if(game_managers.begin(), game_managers.end(), dead),
                      game_managers.end());
}

GameManager *GamesHandler::create_game(const uint8_t &player_id, const uint8_t &world_id,
                                       ServerSender &sender, ServerReceiver &receiver) {
  std::lock_guard<std::mutex> lck(m);
  std::string world_name = worlds_list[world_id];
  World world = worlds_reader.generate_world(world_name);

  GameManager *game_man = new GameManager(games_counter, world, games_config);
  game_man->add_player(sender, player_id);
  receiver.set_game_commands_queue(game_man->get_commands_queue());

  games_counter++;
  game_managers.push_back(game_man);

  std::string str = "[CLIENT-MAN-THREAD]: Client of id: " +
                    std::to_string(static_cast<int>(player_id)) + " created game id: " +
                    std::to_string(static_cast<int>(game_man->get_game_id())) + "\n";
  std::cout << str;

  return game_man;
}

GameManager *GamesHandler::join_game(const uint8_t &player_id, const uint8_t &game_id,
                                     ServerSender &sender, ServerReceiver &receiver) {
  std::lock_guard<std::mutex> lck(m);
  GameManager *game_man = get_game(game_id);
  if (game_man == nullptr) {
    throw std::runtime_error("Error: No se encontró el juego al que se quiso unir.");
  }

  if (!game_man->has_started()) {
    game_man->add_player(sender, player_id);
    receiver.set_game_commands_queue(game_man->get_commands_queue());
    std::string str = "[CLIENT-MAN-THREAD]: Client of id: " +
                      std::to_string(static_cast<int>(player_id)) + " joined game id: " +
                      std::to_string(static_cast<int>(game_man->get_game_id())) + "\n";
    std::cout << str;
  } else {
    throw std::runtime_error("El juego ya ha sido iniciado.");
  }

  return game_man;
}

GameManager *GamesHandler::start_game(const uint8_t &player_id, const uint8_t &game_id) {
  std::lock_guard<std::mutex> lck(m);
  GameManager *game_man = get_game(game_id);
  if (game_man == nullptr) {
    throw std::runtime_error("Error: No se encontró el juego que se quiso iniciar.");
  }

  if (!game_man->has_started()) {
    game_man->initialize_game();
    game_man->start();
    game_man->send_info_to_start_to_players();
  } else {
    throw std::runtime_error("El juego ya ha sido iniciado.");
  }

  return game_man;
}

GameManager *GamesHandler::get_game(const uint8_t &game_id) const {
  auto it = std::find_if(
      game_managers.begin(), game_managers.end(),
      [game_id](GameManager *game_man) { return game_man->compare_id(game_id); });

  if (it != game_managers.end()) {
    return *it;
  }
  return nullptr;
}

bool GamesHandler::game_exist(uint8_t game_id) {
  std::lock_guard<std::mutex> lck(m);

  auto exists = std::any_of(
      game_managers.begin(), game_managers.end(),
      [game_id](GameManager *game_man) { return game_man->compare_id(game_id); });
  return exists;
}

void GamesHandler::reap_dead() {
  auto dead = [](GameManager *game_man) {
    if (game_man->is_dead()) {
      if (game_man->has_started()) {
        game_man->join();
      }
      delete game_man;
      return true;
    }
    return false;
  };

  game_managers.erase(std::remove_if(game_managers.begin(), game_managers.end(), dead),
                      game_managers.end());
}

std::map<uint8_t, std::string> GamesHandler::get_worlds_list() const {
  return worlds_list;
}

GamesHandler::~GamesHandler() {
  for (auto &current_game : game_managers) {
    if (current_game->has_started()) {
      current_game->stop();
      current_game->join();
    }
    delete current_game;
  }
  game_managers.clear();
}

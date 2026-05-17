#include "client_manager_thread.h"

#include <utility>
#include <vector>
#include <string>

#define QUEUE_MAX_SIZE 20

ClientManager::ClientManager(Socket &&peer, GamesHandler &games_handler, uint8_t client_id) :
    games_handler(games_handler), protocol(std::move(peer)), client_id(client_id),
    sender(protocol, keep_playing, client_id),
    receiver(client_id, protocol, keep_playing, in_game, m, is_empty), player(client_id),
    lobby_commands_queue(receiver.get_lobby_commands_queue()) {}

void ClientManager::run() {
  try {
    bool was_closed = false;
    // Loop en el lobby
    while (!in_game) {
      if (!was_closed) {
        lobby_command_ptr lobby_command = protocol.process_command_lobby(&was_closed);
        if (lobby_command) {
          lobby_command->run(*this);

          in_game = player.has_game_started();
          keep_playing = !(player.has_game_finished());
        }
      } else {
        std::cout << "Terminó en el Lobby.\n";
        return;
      }
    }
    auto str =
        "Cliente de id " + std::to_string(static_cast<int>(client_id)) + " entro al juego.\n";
    std::cout << str;

    // Loop en el juego
    while (!has_ended()) {
      sleep(1);
      // is_empty.notify_all();
      // keep_playing = !has_ended();
    }
    finish(was_closed);
    auto str2 = "ClienteManager de id: " + std::to_string(static_cast<int>(client_id)) +
                " llego al final.\n";
    std::cout << str2;
  } catch (const std::exception &e) {
    std::string str = "Error en ClientManager de cliente " + std::to_string(client_id) + ": " +
                      std::string(e.what()) + '\n';
    std::cerr << str;
  }
}

void ClientManager::manage_create_game() {
  auto worlds_map = games_handler.get_worlds_map();
  uint8_t world_id = sender.send_create_info(player.get_id(), worlds_map);

  set_to_host();
  Game *game = games_handler.create_game(player, world_id, sender, receiver);

  sender.send_id(game->get_game_id());
}

void ClientManager::manage_join_game(const uint8_t &game_id) {
  try {
    games_handler.join_game(player, game_id, sender, receiver);
  } catch (std::exception &e) {
    sender.send_id(255);
    return;
  }
  sender.send_id(player.get_id());


  // sender.send_id(game->get_game_id());
  // sender.send_world(game->get_world());
  // sender.send_join_info(player.get_id(), game->get_world());
}

void ClientManager::manage_start_game(const uint8_t &game_id) {
  games_handler.start_game(player, game_id);
  // sender.send_world(game->get_world());
  sender.start();
  receiver.wait_for_client_ready();
  receiver.start();
}

void ClientManager::manage_game_started() {
  sender.start();
  receiver.wait_for_client_ready();
  receiver.start();
  std::cout << "Se recibio el game started\n";
}

void ClientManager::set_to_host() { is_host = true; }

bool ClientManager::has_ended() { return threads_have_finished() || (player.has_game_finished()); }

bool ClientManager::threads_have_finished() {
  return !(receiver.is_alive()) || !(sender.is_alive());
}

void ClientManager::finish(bool was_closed) {
  keep_playing = false;
  // protocol.close_socket(sender.is_closed());
  player.push_last_game_state();
  sender.join();
  receiver.join();
}

void ClientManager::kill() {
  if (is_alive()) {
    std::cout << "ClientHandler muere" << std::endl;
    finish(false);
    keep_playing = false;
    // protocol.close_socket();
  }
}

bool ClientManager::is_dead() { return !keep_playing; }

#include "client_manager_thread.h"

#include <utility>
#include <vector>

#define QUEUE_MAX_SIZE 20

ClientManager::ClientManager(Socket &&peer, GamesHandler &games_handler, uint8_t client_id) :
    games_handler(games_handler), protocol(std::move(peer)), sender(protocol, keep_playing), client_id(client_id),
    receiver(client_id, protocol, keep_playing, in_game, m, is_empty), player(client_id),
    lobby_commands_queue(receiver.get_lobby_commands_queue()) {}

void ClientManager::run() {
  try {
    receiver.start();

    // Loop en el lobby
    while (!in_game) {
      if (keep_playing) {
        std::shared_ptr<RunnableCommandLobby> runnable_lobby_command;
        if (lobby_commands_queue.try_pop(runnable_lobby_command)) {
          std::lock_guard<std::mutex> lck(m);
          runnable_lobby_command->run(*this);
          in_game = player.has_game_started();
        }
        is_empty.notify_all();

        keep_playing = !(player.has_game_finished());
      } else {
        std::cout << "Terminó en el Lobby" << std::endl;
        return;
      }
    }

    // auto &commands_queue_game = player->get_commands_queue_game();
    // set_commands_queue_to_receiver(&commands_queue_game);
    if (!sender.has_started()) {
      sender.start();  // Para asegurarme de que el resto de senders, ademas del que comenzó la partida, empiecen.
    }

    is_empty.notify_all();
    // Loop en el juego
    while (!has_ended()) {
      sleep(1);
      is_empty.notify_all();
      // keep_playing = !has_ended();
    }
    finish();
    std::cout << "ClientManager llego al final" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error en ClientManager: " << e.what() << '\n';
  }
}

void ClientManager::manage_create_game() {
  auto worlds_map = games_handler.get_worlds_map();
  uint8_t world_id = sender.send_create_info(player.get_id(), worlds_map);

  Game *game = games_handler.create_game(player, world_id, sender.get_queue(), receiver.get_game_commands_queue());

  sender.send_id(game->get_game_id());
  sender.send_world(game->get_world());
}

void ClientManager::manage_join_game(const uint8_t &game_id) {
  Game *game = games_handler.join_game(player, game_id, sender.get_queue());
  sender.send_join_info(player.get_id(), game->get_world());
}

void ClientManager::manage_start_game(const uint8_t &game_id) {
  games_handler.start_game(player, game_id);
  sender.start();
  receiver.wait_for_client_ready();
}

bool ClientManager::has_ended() { return threads_have_finished() || (player.has_game_finished()); }

bool ClientManager::threads_have_finished() { return !(receiver.is_alive()) || !(sender.is_alive()); }

void ClientManager::finish() {
  keep_playing = false;
  sender.join();
  receiver.join();
  protocol.close_socket();
}

void ClientManager::kill() {
  if (is_alive()) {
    std::cout << "ClientHandler muere" << std::endl;
    finish();
    keep_playing = false;
    // protocol.close_socket();
  }
}

bool ClientManager::is_dead() { return !keep_playing; }

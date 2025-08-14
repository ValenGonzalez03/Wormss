#include "client_manager_thread.h"

#include <utility>
#include <vector>

#define QUEUE_MAX_SIZE 20

ClientManager::ClientManager(Socket &&peer, GamesHandler &games_handler, uint8_t client_id)
    : skt(std::move(peer)), sender_queue(std::make_shared<Queue<GameState>>(QUEUE_MAX_SIZE)),
      games_handler(games_handler), protocol(std::move(skt)),
      sender(protocol, sender_queue, keep_playing), client_id(client_id),
      receiver(std::make_unique<ServerReceiver>(skt, protocol, lobby_commands_queue, keep_playing, in_game, m, is_empty, client_id)), 
      player(std::make_shared<Player>(client_id, nullptr, games_handler, sender, sender_queue, protocol)), 
      lobby_commands_queue(QUEUE_MAX_SIZE)
      {}

void ClientManager::run() {
  try
  {
    receiver->start();
  
    while (not in_game) {
      if (keep_playing) {
        std::shared_ptr<RunnableCommandLobby> runnable_lobby_command;
        if (lobby_commands_queue.try_pop(runnable_lobby_command)) {
          //std::cout << "Se ejecuto un comando de lobby" << std::endl;
          std::lock_guard<std::mutex> lck(m);
          runnable_lobby_command->run(player);
          in_game = player->has_game_started();
        }
        is_empty.notify_all();
  
        keep_playing = !(player->has_game_finished());
      } else {
        std::cout << "Terminó en el Lobby" << std::endl;
        return;
      }
    }
  
    auto& commands_queue_game = player->get_commands_queue_game();
    set_commands_queue_to_receiver(&commands_queue_game);
    if (!sender.has_started()) {
      sender.start(); // Hago esto para asegurarme de que el resto de senders, ademas del que comenzó la partida, empiecen.
    }
    
    while (keep_playing) {
      is_empty.notify_all();
      keep_playing = !has_ended();
    }
    finish();
  }
  catch(const std::exception& e)
  {
    std::cerr << "Error en ClientManager: " << e.what() << '\n';
  }
  
}
bool ClientManager::has_ended() {
  return threads_have_finished() || (player->has_game_finished());
}

bool ClientManager::threads_have_finished() {
  return !(receiver->is_alive()) || !(sender.is_alive());
}


void ClientManager::finish() {
  protocol.close_socket();
  sender.join();
  receiver->join();
}

void ClientManager::kill() {
  if (is_alive()) {
    std::cout << "ClientHandler muere" << std::endl;
    finish();
    keep_playing = false;
    //protocol.close_socket();
  }
}

bool ClientManager::is_dead() { return not keep_playing; }

void ClientManager::set_commands_queue_to_receiver(Queue<std::shared_ptr<RunnableCommandGame>> *commands_queue) {
  receiver->set_commands_queue(commands_queue);
}

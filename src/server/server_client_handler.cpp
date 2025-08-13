#include "server_client_handler.h"

#include <utility>
#include <vector>

#define QUEUE_MAX_SIZE 20

ClientHandler::ClientHandler(Socket &&peer, GamesHandler &games_handler, uint8_t client_id)
    : skt(std::move(peer)), sender_queue(std::make_shared<Queue<GameState>>(QUEUE_MAX_SIZE)),
      games_handler(games_handler), protocol(std::move(skt)),
      sender(protocol, sender_queue, keep_playing), client_id(client_id),
      receiver(std::make_unique<ServerReceiver>(skt, protocol, lobby_commands_queue, keep_playing, in_game, m, is_empty, client_id)), 
      player(std::make_shared<Player>(client_id, nullptr, games_handler, sender, sender_queue, protocol)), 
      lobby_commands_queue(QUEUE_MAX_SIZE)
      {}

void ClientHandler::start() {
  receiver->start();

  while (not in_game) {
    if (keep_playing) {
      std::shared_ptr<RunnableCommandLobby> runnable_lobby_command;
      if (lobby_commands_queue.try_pop(runnable_lobby_command)) {
        //std::cout << "Se ejecuto un comando de lobby" << std::endl;
        std::lock_guard<std::mutex> lck(m);
        runnable_lobby_command->run(player);
      }
      is_empty.notify_all();

      in_game = player->has_game_started();
      keep_playing = !(player->has_game_finished());
    } else {
      std::cout << "Cae aca?" << std::endl;
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
    keep_playing = !(player->has_game_finished());
  }
}

void ClientHandler::finish() {
  protocol.close_socket();
  sender.join();
  receiver->join();
}

void ClientHandler::kill() {
  if (not is_dead()) {
    std::cout << "ClientHandler muere" << std::endl;
    keep_playing = false;
    //protocol.close_socket();
  }
}

bool ClientHandler::is_dead() { return not keep_playing; }

void ClientHandler::set_commands_queue_to_receiver(Queue<std::shared_ptr<RunnableCommandGame>> *commands_queue) {
  receiver->set_commands_queue(commands_queue);
}

// void ClientHandler::manage_create_game() {
//   protocol.send_id(client_id);

//   bool was_closed = false;
//   auto world_names = games_handler.get_world_names();
//   sender.send_worlds_names(world_names);
//   uint8_t world_id = protocol.recv_world_id(&was_closed);

//   Game *game = games_handler.create_game(sender_queue, client_id, world_id); // Asigno commands_queue a Game y sender_queue al broadcaster.
//   Queue<std::shared_ptr<RunnableCommandGame>>& commands_queue = game->get_commands_queue();
//   set_commands_queue_to_receiver(&commands_queue); // Asigno commands_queue al receiver.
//   this->player = Player(client_id, game);

//   sender.send_id(game->get_game_id()); // Ahora mando el game_id despues (Para tener en cuenta en el cliente)
//   sender.send_world(game->get_world());
//   std::cout << "Created game id: " << int(game->get_game_id()) << std::endl;
// }

// void ClientHandler::manage_join_game(uint8_t game_id) {
//   protocol.send_id(client_id);

//   Game *game = games_handler.join_game(sender_queue, game_id, client_id);
//   std::cout << "Join game id: " << int(game_id) << std::endl;
//   sender.send_world(game->get_world());

//   Queue<std::shared_ptr<RunnableCommandGame>> &commands_queue = game->get_commands_queue();
//   set_commands_queue_to_receiver(&commands_queue); // Asigno commands_queue al receiver.
//   this->player = Player(client_id, game);
//   //in_game = true;
//   //sender.start();
// }

// void ClientHandler::manage_start_game() {
//   bool was_closed = false;
//   player.initialize_game();
//   while (not protocol.recv_client_ready(&was_closed)) {
//     ;
//   }
//   sender.start();
//   games_handler.start_game(player.get_game_id(), client_id);
// }
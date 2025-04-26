#include "client_handler_thread.h"

ClientHandler::ClientHandler(Socket &skt, ServerProtocol &protocol,
                             GamesHandler &games_handler, PlayerSender &sender,
                             std::shared_ptr<Queue<GameState>> sender_queue,
                             std::atomic<bool> &keep_playing,
                             std::atomic<bool> &in_game, uint8_t player_id)
    : skt(skt), protocol(protocol), games_handler(games_handler),
      sender(sender), sender_queue(sender_queue), keep_playing(keep_playing),
      in_game(in_game), player_id(player_id) {}

void ClientHandler::run() {
  bool was_closed = false;
  try {
    while (keep_playing) {
      if (not in_game) { 
        std::shared_ptr<RunnableCommandLobby> runnable_command = protocol.process_command_lobby();

        lobby_result = runnable_command->run(games_handler, sender_queue, player_id);
        if (lobby_result == nullptr) {
          continue;
        }

        if (lobby_result->get_game_started()) {
          sender.start();
          auto game_id = lobby_result->get_game_id();
          auto game = games_handler.get_game(game_id);
          game->charge_world();
          while (not protocol.recv_client_ready(&was_closed)) {
            /* ... */
          }
          in_game = true;
          games_handler.start_game(game_id, player_id);
          continue;
        }
        
        game_commands = lobby_result->get_commands();
        sender.send_id(player_id);
        if (lobby_result->get_game_created()) {
          uint8_t game_id = lobby_result->get_game_id();
          sender.send_id(game_id);
          sender.send_worlds_names(lobby_result->get_worlds_names());
          int world_id = protocol.recv_world_id(&was_closed);
          World selected_world = games_handler.select_world(world_id, game_id);
          sender.send_world(selected_world);
        } else if (lobby_result->get_player_joined()) {
          uint8_t game_id = lobby_result->get_game_id();
          std::cout << "Join game id: " << int(game_id) << std::endl;
          World selected_world = games_handler.select_world(game_id);
          sender.send_world(selected_world);
          in_game = true;
          sender.start();
        }
      } else {
        //std::cout << "id_player: " + std::to_string(player_id) << std::endl;
        std::shared_ptr<RunnableCommandGame> runnable_command = protocol.process_command(player_id);
        game_commands->try_push(runnable_command);
      }
    }

  } catch (const LibError &libError) { // Si se cierra el skt
    keep_playing = false;
  } catch (const std::runtime_error &runtimeError) { // Si se procesa mal un cmd
    keep_playing = false;
    std::cerr << "RuntimeError: " << runtimeError.what() << std::endl;
  }
}

void ClientHandler::join_sender() {
  if (in_game) {
    sender.join();
  }
}

ClientHandler::~ClientHandler() { keep_playing = false; }

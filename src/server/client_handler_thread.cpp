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
  // std::shared_ptr<Command> command = std::make_shared<StartMoving>(0); SOLO
  // PARA PRUEBAS
  try {
    while (keep_playing) {
      if (not in_game) { // comunicacion sincronica
        // std::list<int>* games_id = games_handler.obtain_all_games_id();
        // protocol.send_games_id();

        std::shared_ptr<RunnableCommandLobby> runnable_command =
            protocol.process_command_lobby();

        lobby_result =
            runnable_command->run(games_handler, sender_queue, player_id);
        if (lobby_result == nullptr) {
          continue;
        }

        if (lobby_result->get_game_started()) {
          in_game = true;
          sender.start();
          continue;
        }
        
        game_commands = lobby_result->get_commands();
        sender.send_id(player_id);
        if (lobby_result->get_game_created()) {
          sender.send_id(lobby_result->get_game_id());
        } else if (lobby_result->get_player_joined()) {
          in_game = true;
          sender.start();
        }
      } else { // comunicacion asincronica
        std::cout << "id_player: " + std::to_string(player_id) << std::endl;
        std::shared_ptr<RunnableCommandGame> runnable_command =
            protocol.process_command(player_id);
        game_commands->try_push(runnable_command);
      }
    }

  } catch (const LibError &libError) { // Si se cierra el skt
    keep_playing = false;
    // std::cerr << "LibError: " << libError.what() << std::endl;
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

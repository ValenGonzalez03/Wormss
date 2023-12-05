#ifndef RUNNABLE_CREATE_GAME_H_
#define RUNNABLE_CREATE_GAME_H_

#include "../../common/commands/create_game.h"
#include "command_runnable_lobby.h"

class RunnableCreateGame : public RunnableCommandLobby {

public:
  RunnableCreateGame(int clt_id, Socket &skt, bool *was_closed)
      : RunnableCommandLobby(
            std::make_shared<CreateGame>(clt_id, skt, was_closed)) {}

  std::unique_ptr<LobbyResult>
  run(GamesHandler &games_handler,
      std::shared_ptr<Queue<GameState>> sender_queue, uint8_t &player_id) override {
    uint8_t game_id;
    std::vector<std::string> world_names;
    Queue<std::shared_ptr<RunnableCommandGame>> *commands_queue =
        games_handler.create_game(sender_queue, game_id, player_id, world_names);
    std::unique_ptr<LobbyResult> lobby_result =
        std::make_unique<LobbyResult>(player_id, game_id, commands_queue, world_names);
    lobby_result->game_created();
    return lobby_result;
  }
};

#endif

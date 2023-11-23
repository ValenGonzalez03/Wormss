#ifndef RUNNABLE_START_GAME_H_
#define RUNNABLE_START_GAME_H_

#include "command_runnable_lobby.h"
#include "../common/start_game.h"

class RunnableStartGame : public RunnableCommandLobby {

public:

    RunnableStartGame(int clt_id, Socket &skt, bool *was_closed) : RunnableCommandLobby(std::make_shared<StartGame>(clt_id, skt, was_closed)) {}

    std::unique_ptr<LobbyResult> run(GamesHandler& games_handler, std::shared_ptr<Queue<GameState>> sender_queue, int& game_id, int& player_id) override {
        games_handler.start_game(game_id, player_id);
        std::unique_ptr<LobbyResult> lobby_result = std::make_unique<LobbyResult>(player_id, game_id, nullptr);
        lobby_result->game_started();
        return lobby_result;
    }

};

#endif

#ifndef RUNNABLE_START_GAME_H_
#define RUNNABLE_START_GAME_H_

#include "command_runnable_lobby.h"
#include "../common/start_game.h"

class RunnableStartGame : public RunnableCommandLobby {

public:

    RunnableStartGame(int clt_id, Socket &skt, bool *was_closed) : RunnableCommandLobby(std::make_shared<StartGame>(clt_id, skt, was_closed)) {}

    std::unique_ptr<LobbyResult> run(GamesHandler& games_handler, std::shared_ptr<Queue<GameState>> sender_queue, int& game_id, int& player_id) override {
        std::unique_ptr<LobbyResult> lobby_result = games_handler.create_game(sender_queue, game_id, player_id);
        //sender.send_id(player_id);
        return lobby_result;
    }

};

#endif

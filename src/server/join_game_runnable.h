#ifndef RUNNABLE_JOIN_GAME_H_
#define RUNNABLE_JOIN_GAME_H_

#include "command_runnable_lobby.h"
#include "../common/join_game.h"

class RunnableJoinGame : public RunnableCommandLobby {

public:

    RunnableJoinGame(int clt_id, Socket &skt, bool *was_closed) : RunnableCommandLobby(std::make_shared<JoinGame>(clt_id, skt, was_closed)) {}

    std::unique_ptr<LobbyResult> run(GamesHandler& games_handler, std::shared_ptr<Queue<GameState>> sender_queue, int& game_id, int& player_id) override {
        std::unique_ptr<LobbyResult> lobby_result = games_handler.create_game(sender_queue, game_id, player_id);
        //sender.send_id(player_id);
        return lobby_result;
    }

};

#endif

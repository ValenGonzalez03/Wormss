#ifndef RUNNABLE_JOIN_GAME_H_
#define RUNNABLE_JOIN_GAME_H_

#include "command_runnable_lobby.h"
#include "../common/join_game.h"

class RunnableJoinGame : public RunnableCommandLobby {

public:

    RunnableJoinGame(int clt_id, Socket &skt, bool *was_closed) : RunnableCommandLobby(std::make_shared<JoinGame>(clt_id, skt, was_closed)) {}

    Queue<std::shared_ptr<RunnableCommandGame>>* run(GamesHandler& games_handler, PlayerSender& sender, std::shared_ptr<Queue<GameState>> sender_queue, int& game_id, int& player_id) override {
        Queue<std::shared_ptr<RunnableCommandGame>>* receiver_queue = games_handler.create_game(sender_queue, game_id, player_id);
        sender.send_id(player_id);
        return receiver_queue;
    }

};

#endif

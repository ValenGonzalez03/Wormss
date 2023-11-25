#ifndef RUNNABLE_JOIN_GAME_H_
#define RUNNABLE_JOIN_GAME_H_

#include "command_runnable_lobby.h"
#include "../common/join_game.h"

class RunnableJoinGame : public RunnableCommandLobby {

public:
    RunnableJoinGame(int clt_id, Socket &skt, bool *was_closed) : RunnableCommandLobby(std::make_shared<JoinGame>(clt_id, skt, was_closed)) {}

    std::unique_ptr<LobbyResult> run(GamesHandler& games_handler, std::shared_ptr<Queue<GameState>> sender_queue, uint8_t& game_id, int& player_id) override {
        Queue<std::shared_ptr<RunnableCommandGame>>* commands_queue = games_handler.join_game(sender_queue, game_id, player_id);
        std::unique_ptr<LobbyResult> lobby_result = std::make_unique<LobbyResult>(player_id, game_id, commands_queue);
        lobby_result->player_joined();
        return lobby_result;
    }

    uint8_t get_game_id() {
		return ((JoinGame*)command.get())->get_game_id();
	}
};

#endif

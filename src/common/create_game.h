#ifndef CREATE_GAME_H_
#define CREATE_GAME_H_

#include "command.h"
#include "protocol.h"
#include "../server/server_games_handler.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
    extern uint8_t CREATE_GAME;
}

class CreateGame : public Command {
public:
    // Constructor from client side
    CreateGame() : Command(CODE_PLAYER_COMM::CREATE_GAME, 0) {}

    // Constructor from server side
    CreateGame(int clt_id, Socket &skt, bool *was_closed) : Command(CODE_PLAYER_COMM::CREATE_GAME, clt_id) {
        receive(skt, was_closed);
    }

    void send(Socket &skt, bool* was_closed) override {
        skt.sendall(&code, sizeof(code), was_closed);
    }

    void receive(Socket &skt, bool* was_closed) override {}

    Queue<std::unique_ptr<Command>>* run(GamesHandler& games_handler, std::shared_ptr<Queue<GameState*>> sender_queue) {
        return games_handler.create_game(sender_queue, client_id);
    }
};

#endif

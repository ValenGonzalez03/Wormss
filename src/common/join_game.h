#ifndef JOIN_GAME_H_
#define JOIN_GAME_H_

#include "command.h"
//#include "protocol.h"
//#include "../server/server_games_handler.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
    extern uint8_t JOIN_GAME;
}

class JoinGame : public Command {
private:
    uint8_t game_id;

public:
    // Constructor from client side
    JoinGame(int game_id) : Command(CODE_PLAYER_COMM::JOIN_GAME, 0), game_id(game_id) {}

    // Constructor from server side
    JoinGame(int clt_id, Socket &skt, bool *was_closed) : Command(CODE_PLAYER_COMM::JOIN_GAME, clt_id) {
        //receive(skt, was_closed);
    }

    void send(Socket &skt, bool* was_closed) override {
        skt.sendall(&code, sizeof(code), was_closed);
        skt.sendall(&game_id, sizeof(game_id), was_closed);
    }

    void receive(Socket &skt, bool* was_closed) override {
        skt.recvall(&game_id, sizeof(game_id), was_closed);
    }
	// PROVISORIAS
	uint8_t get_client_id() override {
		return client_id;
	}
	
    uint8_t get_game_id() override {
		return game_id;
	}
};

#endif

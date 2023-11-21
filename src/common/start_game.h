#ifndef START_GAME_H_
#define START_GAME_H_

#include "command.h"
#include "protocol.h"

namespace CODE_PLAYER_COMM {
extern uint8_t START_GAME;
}

class StartGame : public Command {
public:
    // Constructor from client side
    StartGame() : Command(CODE_PLAYER_COMM::START_GAME, 0) {}

    // Constructor from server side
    StartGame(int clt_id, Socket &skt, bool *was_closed) : Command(CODE_PLAYER_COMM::START_GAME, clt_id) {
        //receive(skt, was_closed);
    }

    void send(Socket &skt, bool* was_closed) override {
        skt.sendall(&code, sizeof(code), was_closed);
    }

    #ifdef SERVER_BUILD
    void receive(Socket &skt, bool* was_closed) override {}
    
    void run(GameManager &game_manager) override {
        
    }
    
	/*
    Queue<std::shared_ptr<Command>>* run(GamesHandler& games_handler, std::shared_ptr<Queue<GameState>> sender_queue) override {
        return games_handler.create_game(sender_queue, client_id);
    }*/
    
    bool is_connect_type() override {
		return true;
	}
	
	bool is_create_command() override {
		return true;
	}
	
	bool is_join_command() override {
		return false;
	}
	
	// PROVISORIAS
	uint8_t get_client_id() override {
		return client_id;
	}
	
    uint8_t get_game_id() override {
		return -1;
	}
    #endif
};

#endif

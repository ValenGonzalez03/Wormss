#ifndef STOP_MOVING_H_
#define STOP_MOVING_H_

#include "command.h"
#include "protocol.h"
//#include "../server/server_games_handler.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
	extern uint8_t STOP_MOVING;
}

class StopMoving : public Command {
public:
  // Constructor from client side
  explicit StopMoving() : Command(CODE_PLAYER_COMM::STOP_MOVING, 0) {}

  // Constructor from server side for code consistency but doesn't do anything
  // different from the other
  StopMoving(int clt_id, Socket &skt, bool *was_closed)
      : Command(CODE_PLAYER_COMM::STOP_MOVING, clt_id) {
     //receive(skt, was_closed);
  }

  void send(Socket &skt, bool *was_closed) override {
    skt.sendall(&code, sizeof(code), was_closed);
  }

  #ifdef SERVER_BUILD
  void receive(Socket &skt, bool *was_closed) override {}

  void run(GameManager &game_manager) override {
        std::cout << "Stop Moving" << std::endl;
  }
    /*
    Queue<std::shared_ptr<Command>>* run(GamesHandler& games_handler, std::shared_ptr<Queue<GameState>> sender_queue) override {
        return nullptr;
    }*/
    
  bool is_connect_type() override {
		return false;
	}
	
	bool is_create_command() override {
		return false;
	}
	
	bool is_join_command() override {
		return false;
	}
	
	// PROVISORIAS
	uint8_t get_client_id() override {
		return -1;
	}
	
  uint8_t get_game_id() override {
		return -1;
	}
  #endif
};

#endif

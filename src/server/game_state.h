#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>
#include <vector>
#include <cstdint>


struct WormState {
	private:
	uint16_t pos_x;
	uint16_t pos_y;
	uint16_t id;
	uint16_t player_id;
	uint8_t direction;
	
};


struct GameState {
	private:
	uint16_t players_amount;
	uint16_t worms_amount;
	std::vector<WormState> worms;
	
	public:
	GameState(uint16_t players_amount, uint16_t worms_amount) : players_amount(players_amount), worms_amount(worms_amount) {}
	
};


#endif

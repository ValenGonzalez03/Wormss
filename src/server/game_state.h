#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <list>
#include <vector>
#include <cstdint>


struct WormState {
	private:
	float pos_x;
	float pos_y;
	//uint16_t id;
	//uint16_t player_id;
	//uint8_t direction;
	public:
	WormState(const float& pos_x, const float& pos_y) : pos_x(pos_x), pos_y(pos_y) {}
	
};


struct GameState {
	private:
	//uint16_t players_amount;
	std::list<WormState> worms_list;
	
	public:
	GameState() {}
	
	void add_worm(const float& pos_x, const float& pos_y) {
		WormState worm_state(pos_x, pos_y);
		worms_list.push_back(worm_state);
	}
	
};


#endif

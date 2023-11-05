#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "box2d/box2d.h"
#include <stdio.h>
#include "world.h"
#include "worm.h"
#include "beam.h"

class GameState {
	private:
	World* world;
	int players = 0;
	//current_player_id
	//current_worm_id
	
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	public:
	explicit GameState();
	
	void step();
	
	void update();
	
	void get_state();
	
	 ~GameState();
};

#endif

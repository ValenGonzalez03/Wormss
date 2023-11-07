#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "box2d/box2d.h"
#include <stdio.h>
#include "world.h"
#include "worm.h"
#include "beam.h"

class GameManager {
	private:
	World* world;
	int players = 0;
	//current_player_id
	//current_worm_id
	
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	public:
	explicit GameManager();
	
	void step();
	
	void update();
	
	void get_state();
	
	 ~GameManager();
};

#endif

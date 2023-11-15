#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "box2d/box2d.h"
#include <stdio.h>
#include "world.h"
#include "../common/game_state.h"

class GameManager {
	private:
	World world;
	int current_players = 0;
	int current_player_id;
	int current_worm_id;
	std::list<int> players;
	WormBody* worm;
	
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	public:
	explicit GameManager();
	
	void initialize_game();
	
	void add_player(const int& player_id);
	
	void delete_player(const int& player_id);
	
	void step();
	
	void update();
	
	void move(const int& player_id, const int& direction);
	
	void get_state();
	
	// SOLO DE PRUEBA
	b2Vec2 get_worm_position();
	
	~GameManager();
	
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete; 
};

#endif

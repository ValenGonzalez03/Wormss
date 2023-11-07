#include "box2d/box2d.h"
#include <stdio.h>
#include "game_manager.h"

GameManager::GameManager(World world) {}

void step() {
	world->step(timeStep, velocityIterations, positionIterations);
}

void update() {}

void update_current_worm() {
		//current_worm->move...
}
	
void get_state() {}

GameManager::~GameManager() {
}

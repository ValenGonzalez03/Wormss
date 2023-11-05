#include "box2d/box2d.h"
#include <stdio.h>

GameState::GameState(World world) {}

void step() {
	world->step(timeStep, velocityIterations, positionIterations);
}

void update() {}

void update_current_worm() {
		//current_worm->move...
}
	
void get_state() {}

GameState::~GameState() {
}

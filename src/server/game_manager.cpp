#include "box2d/box2d.h"
#include <stdio.h>
#include "game_manager.h"

// cambiar despues
#define LEFT 0
#define RIGHT 1

GameManager::GameManager() {}

void GameManager::initialize_game() {
	world.create_beam(0, 0);
	worm = world.create_worm(0, 4);
	world.create_worm(1, 4);
}

void GameManager::add_player(const int& player_id) {
	current_players++;
	players.push_back(player_id);
}

void GameManager::delete_player(const int& player_id) {
	current_players--;
	players.remove(player_id);
}

void GameManager::step() {
	world.step(timeStep, velocityIterations, positionIterations);
}

void GameManager::update() {}

void GameManager::move(const int& player_id, const int& direction) {
	//obtain worm
	if (direction == LEFT) {
		worm->move_left();
	} else if (direction == RIGHT) {
		worm->move_right();
	}
}
	
GameState GameManager::get_state() {
	GameState game_state(0, 0);
	return game_state;
}

// SOLO DE PRUEBA
b2Vec2 GameManager::get_worm_position() {
	return worm->get_position();
}

GameManager::~GameManager() {
}

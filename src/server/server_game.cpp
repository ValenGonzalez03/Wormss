#include "server_game.h"
#include <chrono>
#include "math.h"
#include <unistd.h>

Game::Game(int& game_id): game_id(game_id) {}

Queue<Command*>* Game::add_player(Queue<GameState*>* sender_queue) {
    queues_list.push_back(sender_queue);
    return commands;
}

void Game::delete_player() {
	/*
    for (auto current_player = players.begin();
         current_player != players.end(); current_player++) {
        if (*current_player == player) {
            players.erase(current_player);
        }
    }
    */
}

void Game::push_command(Command* command) {
    commands->push(command);
}

void Game::run() {
	auto time_start = std::chrono::high_resolution_clock::now();
	int it = 0;
	
	while (keep_playing) {
		update(it);
		
		auto time_end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = time_end - time_start;
		double elapsed = duration.count();
		auto rest = rate - elapsed;
		
		if (rest < 0) {
			double behind = -rest;
			double lost = behind - fmod(behind, rate);
			time_start += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(std::chrono::duration<double> (lost));
			it += int(lost/rate);
		} else {
			sleep(rest);
		}
		time_start += std::chrono::duration_cast<std::chrono::high_resolution_clock::duration>(std::chrono::duration<double> (rate));
		it += 1;
	}
	
}

void Game::update(int& it) {
}

void Game::stop() {
	keep_playing = false;
}

bool Game::compare_id(const int& another_game_id) {
	return (game_id == another_game_id);
}

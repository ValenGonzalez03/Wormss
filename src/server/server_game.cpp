#include "server_game.h"
#include <chrono>
#include "math.h"
#include <unistd.h>

Game::Game() {}

void Game::add_player(Player* player) {
    players.push_back(player);
}

void Game::delete_player(const Player* player) {
    for (auto current_player = players.begin();
         current_player != players.end(); current_player++) {
        if (*current_player == player) {
            players.erase(current_player);
        }
    }
}

void Game::push_command(std::vector<char> command) {
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



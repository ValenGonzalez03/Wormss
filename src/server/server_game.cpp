#include "server_game.h"
#include <chrono>
#include "math.h"
#include <unistd.h>

Game::Game(int& game_id): game_id(game_id), commands(100) {
	game_manager.initialize_game();
	}

Queue<std::unique_ptr<Command>>* Game::add_player(std::shared_ptr<Queue<GameState*>> sender_queue, const int& player_id) {
    queues_sender[player_id] = sender_queue;
    game_manager.add_player(player_id);
    return &commands;
}

void Game::delete_player(const int& player_id) {
	queues_sender.erase(player_id);
	game_manager.delete_player(player_id);
}

void Game::push_command(std::unique_ptr<Command> command) {
    commands.push(command);
}

void Game::run() {
	auto time_start = std::chrono::high_resolution_clock::now();
	int it = 0;
	
	while (keep_playing) {
		update(it);
		game_manager.step();
		
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
	std::unique_ptr<Command> command;
	while (commands.try_pop(command)) {
		//commands.try_pop(command);
		//command.handle_command(game_manager);
		it--;
	}
}

void Game::stop() {
	keep_playing = false;
}

bool Game::compare_id(const int& another_game_id) {
	return (game_id == another_game_id);
}

void Game::push_game_state(GameState* game_state) {	//hacer monitor luego
	std::lock_guard<std::mutex> lck(m);
	for (auto& current_queue: queues_sender) {
        current_queue.second->push(game_state);
	}
}

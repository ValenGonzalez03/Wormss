#include "server_game.h"
#include <chrono>
#include "math.h"
#include <unistd.h>

Game::Game(int& game_id): game_id(game_id), commands(100) {
	game_manager.initialize_game();
	}

Queue<std::shared_ptr<Command>>* Game::add_player(std::shared_ptr<Queue<GameState>> sender_queue, const int& player_id) {
    queues_sender[player_id] = sender_queue;
    game_manager.add_player(player_id);
    
    GameState game_state = game_manager.get_state();
    sender_queue->try_push(game_state);
    return &commands;
}

void Game::delete_player(const int& player_id) {
	queues_sender.erase(player_id);
	game_manager.delete_player(player_id);
}

void Game::run() { try {
	auto time_start = std::chrono::high_resolution_clock::now();
	int it = 0;
	
	while (keep_playing) {
		update(it);
		game_manager.step();
		
		push_game_state(game_manager.get_state());
		
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
}	catch (const std::exception& err) {
	
	} 
}

void Game::update(int& it) {
	std::shared_ptr<Command> command;
	while (commands.try_pop(command)) {
		commands.try_pop(command);
		command->run();
		it--;
	}
}

void Game::stop() {
	keep_playing = false;
}

bool Game::compare_id(const int& another_game_id) {
	return (game_id == another_game_id);
}

void Game::push_game_state(GameState game_state) {	// hacer monitor, posible RC
	std::lock_guard<std::mutex> lck(m);
	for (auto& current_queue: queues_sender) {
        current_queue.second->try_push(game_state);
	}
}

bool Game::is_dead() {
	return not keep_playing;
}

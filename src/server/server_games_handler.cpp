#include "server_games_handler.h"

GamesHandler::GamesHandler() {}

void GamesHandler::add_game(Game* game) {
    games.push_back(game);
}

void GamesHandler::delete_game(const int& game_id) {
	std::lock_guard<std::mutex> lck(m);
    
    for (auto current_game: games) {
        if (current_game->compare_id(game_id)) {
			current_game->stop();
			current_game->join();
			delete current_game;
		}
    }
    games.clear();
}

Queue<Command*>* GamesHandler::create_game(Queue<GameState*>* sender_queue, const int& player_id) {
	std::lock_guard<std::mutex> lck(m);
	Game* game = new Game(games_counter);
	games_counter++;
	add_game(game);
	game->start();
	return game->add_player(sender_queue);
}

Queue<Command*>* GamesHandler::join_game(Queue<GameState*>* sender_queue, const int& player_id, const int& game_id) {
	std::lock_guard<std::mutex> lck(m);
	for (auto& current_game: games) {
        if (current_game->compare_id(game_id)) {
			return current_game->add_player(sender_queue);
		}
    }
    return nullptr;
}

bool GamesHandler::game_exist(int game_id) {
	std::lock_guard<std::mutex> lck(m);
	for (auto& current_game: games) {
        if (current_game->compare_id(game_id)) {
			return true;
		}
    }
    return false;
}

GamesHandler::~GamesHandler() {
	 for (auto& current_game: games) {
		current_game->stop();
		current_game->join();
        delete current_game;
    }
    games.clear();
}

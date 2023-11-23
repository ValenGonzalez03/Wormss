#include "server_games_handler.h"

GamesHandler::GamesHandler() {}

void GamesHandler::add_game(Game* game) {
    games.push_back(game);
}

void GamesHandler::delete_game(const int& game_id) {
	std::lock_guard<std::mutex> lck(m);
	auto dead = [game_id](Game* game) {
        if (game->compare_id(game_id)) {
			game->stop();
            game->join();
            delete game;
            return true;
        }
        return false;
    };
    
    games.erase(std::remove_if(games.begin(), games.end(), dead), games.end());
}

std::unique_ptr<LobbyResult> GamesHandler::create_game(std::shared_ptr<Queue<GameState>> sender_queue, int& game_id, int& player_id) {
	std::lock_guard<std::mutex> lck(m);
	Game* game = new Game(games_counter);
    game_id = games_counter;
	games_counter++;
	add_game(game);
	game->start();
	game->add_player(sender_queue, player_id);
	return std::make_unique<LobbyResult>(player_id, game_id, game->add_player(sender_queue, player_id));
}

std::unique_ptr<LobbyResult> GamesHandler::join_game(std::shared_ptr<Queue<GameState>> sender_queue, const int& game_id, int& player_id) {
	std::lock_guard<std::mutex> lck(m);
	Queue<std::shared_ptr<RunnableCommandGame>>* commands_queue = nullptr;
	for (auto& current_game: games) {
        if (current_game->compare_id(game_id)) {
			if (!current_game->is_started()) {
				commands_queue = current_game->add_player(sender_queue, player_id);
			}
		}
    }
    return std::make_unique<LobbyResult>(player_id, game_id, commands_queue);
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

void GamesHandler::reap_dead() {
	auto dead = [](Game* game) {
        if (game->is_dead()) {
            game->join();
            delete game;
            return true;
        }
        return false;
    };
    
    games.erase(std::remove_if(games.begin(), games.end(), dead), games.end());
}

std::list<int>* GamesHandler::obtain_all_games_id() {
	std::list<int>* games_id;
	for (auto& current_game: games) {
		games_id->push_back(current_game->get_game_id());
    }
    return games_id;
}

GamesHandler::~GamesHandler() {
	for (auto& current_game: games) {
		current_game->stop();
		current_game->join();
        delete current_game;
    }
    games.clear();
}

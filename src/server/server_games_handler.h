#ifndef SERVER_GAMES_HANDLER_H
#define SERVER_GAMES_HANDLER_H

#include <list>
#include <mutex>
#include <algorithm>

#include "server_game.h"
#include "command_runnable_game.h"
#include "lobby_result.h"

class Command;

class GamesHandler {
private:
    std::mutex m;
    std::list<Game*> games;	
    int games_counter = 0;

public:
    GamesHandler();

    void add_game(Game* game);

    void delete_game(const int& game_id);
	
	std::unique_ptr<LobbyResult> create_game(std::shared_ptr<Queue<GameState>> sender_queue, int& game_id, int& player_id);

	std::unique_ptr<LobbyResult> join_game(std::shared_ptr<Queue<GameState>> sender_queue, const int& game_id, int& player_id);
    
    bool game_exist(int game_id);
    
    void reap_dead();
    
    std::list<int>* obtain_all_games_id();
    
    ~GamesHandler();
    
    GamesHandler(const GamesHandler&) = delete;
	GamesHandler& operator=(const GamesHandler&) = delete; 
};

#endif

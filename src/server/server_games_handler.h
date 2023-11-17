#ifndef SERVER_GAMES_HANDLER_H
#define SERVER_GAMES_HANDLER_H

#include <list>
#include <mutex>
#include <algorithm>

#include "server_game.h"

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

    Queue<std::shared_ptr<Command>>* create_game(std::shared_ptr<Queue<GameState>> sender_queue, const int&  player_id);

    Queue<std::shared_ptr<Command>>* join_game(std::shared_ptr<Queue<GameState>> sender_queue, const int&  player_id, const int&  game_id);
    
    bool game_exist(int game_id);
    
    void reap_dead();
    
    ~GamesHandler();
    
    GamesHandler(const GamesHandler&) = delete;
	GamesHandler& operator=(const GamesHandler&) = delete; 
};

#endif

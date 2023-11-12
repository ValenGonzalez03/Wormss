#ifndef SERVER_GAMES_HANDLER_H
#define SERVER_GAMES_HANDLER_H

#include <list>
#include <mutex>

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

    Queue<Command*>* create_game(Queue<GameState*>* sender_queue, const int&  player_id);

    Queue<Command*>* join_game(Queue<GameState*>* sender_queue, const int&  player_id, const int&  game_id);
    
    bool game_exist(int game_id);
    
    ~GamesHandler();
    
    GamesHandler(const GamesHandler&) = delete;
	GamesHandler& operator=(const GamesHandler&) = delete; 
};

#endif

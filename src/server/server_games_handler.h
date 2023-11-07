#ifndef SERVER_GAMES_HANDLER_H
#define SERVER_GAMES_HANDLER_H

#include <list>
#include <mutex>

#include "server_game.h"
#include "commands/command.h"

class GamesHandler {
private:
    std::mutex m;
    std::list<Game*> games;

public:
    GamesHandler();

    void add_game(Game* game);

    void delete_game(const Game* game);

    Queue<Command*>* create_game(Queue<GameState*>* sender_queue, int player_id);

    Queue<Command*>* join_game(Queue<GameState*>* sender_queue, int player_id, int game_id);
};

#endif

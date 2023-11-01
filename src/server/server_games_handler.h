#ifndef SERVER_GAMES_HANDLER_H
#define SERVER_GAMES_HANDLER_H

#include <list>
#include <mutex>

#include "server_game.h"

class GamesHandler {
private:
    std::mutex m;
    std::list<Game*> games;

public:
    GamesHandler();

    void add_game(Game* game);

    void delete_game(const Game* game);

    Game* create_game(Player* player);

    void join_game(Player* player);
};

#endif

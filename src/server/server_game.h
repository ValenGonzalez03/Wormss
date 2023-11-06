#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <list>
#include <mutex>

#include "player.h"
#include "../common/thread.h"
#include "../common/queue.h"

#define MAX_PLAYERS 10

class Game : public Thread {
private:
    std::mutex m;
    std::list<Player*> players;
    Queue<std::vector<char>>* commands;
    bool keep_playing = true;
    double rate = 0.1;

public:
    Game();

    void add_player(Player* player);

    void delete_player(const Player* player);

    void push_command(std::vector<char> command);

    void handle_command();
    
    void run();
    
    void update(int& it);
    
    void stop();
};

#endif

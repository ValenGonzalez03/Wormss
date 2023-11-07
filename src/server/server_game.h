#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <list>
#include <mutex>

#include "game_state.h"
#include "commands/command.h"
#include "../common/thread.h"
#include "../common/queue.h"

#define MAX_PLAYERS 10

class Game : public Thread {
private:
    std::mutex m;
    std::list<Queue<GameState*>*> queues_list;
    Queue<Command*>* commands;
    bool keep_playing = true;
    double rate = 0.1;

public:
    Game();

    Queue<Command*>* add_player(Queue<GameState*>* sender_queue);

    void delete_player();

    void push_command(Command* command);

    void handle_command();
    
    void run();
    
    void update(int& it);
    
    void stop();
};

#endif

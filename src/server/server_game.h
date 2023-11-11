#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <list>
#include <mutex>
#include <map>

#include "game_state.h"
#include "game_manager.h"
#include "../common/thread.h"
#include "../common/queue.h"

#define MAX_PLAYERS 10
#define MS_PER_UPDATE 10

class Command;

class Game : public Thread {
private:
    std::mutex m;
    std::map<int,Queue<GameState*>*> queues_sender;
    Queue<Command*> commands;
    int game_id;
    bool keep_playing = true;
    GameManager game_manager;
    double rate = 0.1;

public:
    Game(int& game_id);

    Queue<Command*>* add_player(Queue<GameState*>* sender_queue, const int& player_id);

    void delete_player(const int& player_id);

    void push_command(Command* command);

    void handle_command();
    
    void run();
    
    void update(int& it);
    
    void stop();
    
    bool compare_id(const int& another_game_id);
    
    void push_game_state(GameState* game_state);
    
    Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
};

#endif

#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <list>
#include <mutex>
#include <map>

#include "../common/game_state.h"
#include "../common/command.h"
#include "game_manager.h"
#include "../common/thread.h"
#include "../common/queue.h"

#define MAX_PLAYERS 10
#define MS_PER_UPDATE 10

//class Command;

class Game : public Thread {
private:
    std::mutex m;
    std::map<int,std::shared_ptr<Queue<GameState>>> queues_sender;
    Queue<std::shared_ptr<Command>> commands;
    int game_id;
    bool keep_playing = true;
    GameManager game_manager;
    double rate = 0.1;

public:
    Game(int& game_id);

    Queue<std::shared_ptr<Command>>* add_player(std::shared_ptr<Queue<GameState>> sender_queue, const int& player_id);

    void delete_player(const int& player_id);

    void handle_command();
    
    void run();
    
    void update(int& it);
    
    void stop();
    
    bool compare_id(const int& another_game_id);
    
    void push_game_state(GameState game_state);
    
    bool is_dead();
    
    Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
};

#endif

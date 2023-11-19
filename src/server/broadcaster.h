#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <mutex>
#include <map>
#include <algorithm>

#include "../common/queue.h"
#include "../common/game_state.h"
#include "../common/command.h"

//class Command;

class Broadcaster {
private:
    std::mutex m;
    std::map<int,std::shared_ptr<Queue<GameState>>> queues_sender;

public:
    Broadcaster();
	
	void add_queue(const std::shared_ptr<Queue<GameState>> sender_queue, const int &player_id);
	
	void delete_queue(const int &player_id);
	
	void broadcast(const GameState& game_state);
    
    Broadcaster(const Broadcaster&) = delete;
	Broadcaster& operator=(const Broadcaster&) = delete;
};

#endif

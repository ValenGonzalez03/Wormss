#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <algorithm>
#include <map>
#include <mutex>

#include "../../common/commands/command.h"
#include "../../common/game_state.h"
#include "../../common/lib/queue.h"

class Broadcaster {
private:
  std::mutex m;
  std::map<uint8_t, std::shared_ptr<Queue<GameState>>> queues_sender;

public:
  Broadcaster();

  void add_queue(const std::shared_ptr<Queue<GameState>> sender_queue,
                 const uint8_t &player_id);

  void delete_queue(const uint8_t &player_id);

  void broadcast(const GameState &game_state);

  Broadcaster(const Broadcaster &) = delete;
  Broadcaster &operator=(const Broadcaster &) = delete;
};

#endif

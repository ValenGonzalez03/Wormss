#include "broadcaster.h"

Broadcaster::Broadcaster() {}

void Broadcaster::add_queue(const std::shared_ptr<Queue<GameState>> sender_queue, const uint8_t &player_id) {
  std::lock_guard<std::mutex> lck(m);
  queues_sender[player_id] = sender_queue;
}

void Broadcaster::delete_queue(const uint8_t &player_id) {
  std::lock_guard<std::mutex> lck(m);
  queues_sender.erase(player_id);
}

void Broadcaster::broadcast(const GameState& game_state) {
  std::lock_guard<std::mutex> lck(m);
  for (auto& current_queue: queues_sender) {
	  current_queue.second->try_push(game_state);
  }
}

#include "server_accept_thread.h"
#include "../common/queue.h"

#include <algorithm>
#include <utility>

#define QUEUE_MAX_SIZE 20

Accept::Accept(Socket &skt) : skt(std::move(skt)) {}

void Accept::run() {
  try {
    while (is_alive) {
      std::shared_ptr<Queue<GameState>> sender_queue =
          std::make_shared<Queue<GameState>>(10);
      std::shared_ptr<Player> player = std::make_shared<Player>(
          std::move(skt.accept()), games_handler, sender_queue, id_counter);
      player->start();
      id_counter++;

      reap_dead();
      games_handler.reap_dead();
      players.push_back(player);
    }

  } catch (const std::exception &err) {
    if (is_alive) {
      std::cerr << "Something went wrong and an exception was caught: "
                << err.what() << "\n";
    }
    is_alive = false;
    kill_all();
  }
}

void Accept::reap_dead() {
  auto dead = [](std::shared_ptr<Player> player) {
    if (player->is_dead()) {
      player->join();
      return true;
    }
    return false;
  };

  players.erase(std::remove_if(players.begin(), players.end(), dead),
                players.end());
}

void Accept::kill_all() {
  for (auto &player : players) {
    player->kill();
    player->join();
  }
  players.clear();
}

void Accept::kill() {
  is_alive = false;
  skt.shutdown(2);
  skt.close();
}

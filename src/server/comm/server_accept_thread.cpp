#include "server_accept_thread.h"
#include "../../common/lib/queue.h"

#include <algorithm>
#include <utility>

#define QUEUE_MAX_SIZE 20

Accept::Accept(Socket &skt) : skt(std::move(skt)) {}

void Accept::run() {
  try {
    while (is_alive) {
      std::shared_ptr<ClientManager> client = std::make_shared<ClientManager>(
          std::move(skt.accept()), games_handler, id_counter);
      client->start();
      id_counter++;

      reap_dead();
      games_handler.reap_dead();
      clients.push_back(client);
    }
  } catch (const std::exception &err) {
    if (is_alive) {
      std::cerr << "Something went wrong and an exception was caught: " << err.what()
                << "\n";
    }
    is_alive = false;
    kill_all();
  }
}

void Accept::reap_dead() {
  auto dead = [](std::shared_ptr<ClientManager> client) {
    if (!client->is_alive()) {
      client->join();
      return true;
    }
    return false;
  };

  // std::cout << "Paso por aca" << std::endl;

  clients.erase(std::remove_if(clients.begin(), clients.end(), dead), clients.end());
}

void Accept::kill_all() {
  for (auto &client : clients) {
    client->kill();
    client->finish(false);
  }
  clients.clear();
}

void Accept::kill() {
  is_alive = false;
  skt.shutdown(2);
  skt.close();
}

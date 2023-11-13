#include "server_accept_thread.h"
#include "../common/queue.h"

#include <algorithm>
#include <utility>

Accept::Accept(Socket& skt): skt(std::move(skt)) {}

void Accept::run() {
    try {
        while (is_alive) {
            Player* player = new Player(skt.accept(), games_handler);
            player->start();

            reap_dead();
            games_handler.reap_dead();
            players.push_back(player);
        }

    } catch (const std::exception& err) {
        if (is_alive) {
            std::cerr << "Something went wrong and an exception was caught: " << err.what() << "\n";
        }
        is_alive = false;
        kill_all();
    }
}

void Accept::reap_dead() {
    auto dead = [](Player* player) {
        if (player->is_dead()) {
            player->join();
            delete player;
            return true;
        }
        return false;
    };

    players.erase(std::remove_if(players.begin(), players.end(), dead), players.end());
}


void Accept::kill_all() {
    for (auto& player: players) {
        player->kill();
        player->join();
        delete player;
    }
    players.clear();
}


void Accept::kill() {
    is_alive = false;
    skt.shutdown(2);
    skt.close();
}

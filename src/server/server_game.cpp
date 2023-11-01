#include "server_game.h"

Game::Game() {}

void Game::add_player(Player* player) {
    players.push_back(player);
}

void Game::delete_player(const Player* player) {
    for (auto current_player = players.begin();
         current_player != players.end(); current_player++) {
        if (*current_player == player) {
            players.erase(current_player);
        }
    }
}

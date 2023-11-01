#include "server_games_handler.h"

GamesHandler::GamesHandler() {}

void GamesHandler::add_game(Game* game) {
    games.push_back(game);
}

void GamesHandler::delete_game(const Game* game) {
    for (auto current_game = games.begin();
         current_game != games.end(); current_game++) {
        if (*current_game == game) {
            games.erase(current_game);
        }
    }
}

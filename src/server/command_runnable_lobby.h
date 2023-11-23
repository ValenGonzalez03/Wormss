#ifndef RUNNABLE_COMMAND_LOBBY_H_
#define RUNNABLE_COMMAND_LOBBY_H_

#include "command_runnable.h"
#include "server_games_handler.h"
#include "lobby_result.h"

class RunnableCommandLobby : RunnableCommand {

public:

    RunnableCommandLobby(std::shared_ptr<Command> command) : RunnableCommand(command) {}

    virtual std::unique_ptr<LobbyResult> run(GamesHandler& games_handler, std::shared_ptr<Queue<GameState>> sender_queue, int& game_id, int& player_id) = 0;

};

#endif

#ifndef RUNNABLE_COMMAND_LOBBY_H_
#define RUNNABLE_COMMAND_LOBBY_H_

#include "command_runnable.h"
#include "server_games_handler.h"
#include "player_sender_thread.h"
//#include "../server/client_handler_thread.h"

class RunnableCommandLobby : RunnableCommand {

public:

    RunnableCommandLobby(std::shared_ptr<Command> command) : RunnableCommand(command) {}

    virtual Queue<std::shared_ptr<RunnableCommandGame>>* run(GamesHandler& games_handler, PlayerSender& sender, std::shared_ptr<Queue<GameState>> sender_queue, int& game_id, int& player_id) = 0;

};

#endif

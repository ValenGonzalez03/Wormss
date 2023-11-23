#ifndef LOBBY_RESULT_H
#define LOBBY_RESULT_H

#include "../common/queue.h"
#include "command_runnable_game.h"

class LobbyResult {
private:
  const int& player_id;
  const int& game_id;
  Queue<std::shared_ptr<RunnableCommandGame>>* commands;
  
public:
  explicit LobbyResult(const int& player_id, const int& game_id, Queue<std::shared_ptr<RunnableCommandGame>>* commands);
  
  void send_id();
  
  void push_command(std::shared_ptr<RunnableCommandGame> runnable_command);

  LobbyResult(const LobbyResult &) = delete;
  LobbyResult &operator=(const LobbyResult &) = delete;
};

#endif

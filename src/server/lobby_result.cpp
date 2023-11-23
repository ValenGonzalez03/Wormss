#include "lobby_result.h"

LobbyResult::LobbyResult(const int& player_id, const int& game_id, Queue<std::shared_ptr<RunnableCommandGame>>* commands):
						player_id(player_id), game_id(game_id), commands(commands) {}

void LobbyResult::send_id() {}

void LobbyResult::push_command(std::shared_ptr<RunnableCommandGame> runnable_command) {
	commands->push(runnable_command);
}

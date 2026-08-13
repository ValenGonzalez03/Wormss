#include "game_manager.h"
#include "../../common/constant_rate_loop.h"
#include <string>

#define QUEUE_MAX_SIZE 20

GameManager::GameManager(const uint8_t &id, const World &world,
                         const GameConfig &game_config) :
    id(id), commands(QUEUE_MAX_SIZE), config(game_config), game(world) {}

void GameManager::add_player(ServerSender &sender, const uint8_t &player_id) {
  broadcaster.add_queue(&sender.get_queue(), player_id);
  player_senders.emplace_back(&sender);
  game.add_player(player_id);
}

void GameManager::delete_player(const uint8_t &player_id) {
  broadcaster.delete_queue(player_id);
  game.delete_player(player_id);
}

void GameManager::initialize_game() {
  game.charge_world(config);
  push_game_state();
}

void GameManager::run() {
  try {
    std::string str =
        "[GAME-THREAD]: Game of id: " + std::to_string(static_cast<int>(id)) +
        " started.\n";
    std::cout << str;

    started = true;

    // bool was_closed = false;
    // int it = 0;
    // auto t1 = time_point_cast<milliseconds>(steady_clock::now());
    auto start_turn_time = std::chrono::steady_clock::now();

    ConstantRateLoop rate_loop(std::chrono::duration<float>(static_cast<float>(1 / FPS)),
                               [this] { return execute_frame(); });
    rate_loop.loop();

    keep_playing = false;

    std::cout << "[GAME-THREAD]: Game of id: " << static_cast<int>(id) << " ended."
              << std::endl;
  } catch (const std::exception &err) {
  }
}

bool GameManager::execute_frame() {
  update();
  game.update();
  push_game_state();

  return has_game_finished();
}

void GameManager::update() {
  std::shared_ptr<RunnableCommandGame> runnable_command;
  while (commands.try_pop(runnable_command)) {
    runnable_command->run(game);
  }
}

void GameManager::push_game_state() {
  GameState game_state = game.create_state();
  broadcaster.broadcast(game_state);
}

bool GameManager::has_game_finished() { return game.is_game_finished(); }

void GameManager::send_info_to_start_to_players() {
  for (auto &sender : player_senders) {
    sender->send_game_started();
    sender->send_world(game.get_world());
  }
}

bool GameManager::compare_id(const uint8_t &another_game_id) {
  return (id == another_game_id);
}

bool GameManager::has_started() { return started; }

bool GameManager::is_dead() { return !keep_playing; }

uint8_t GameManager::get_game_id() { return id; }

Queue<game_command_ptr> *GameManager::get_commands_queue() { return &commands; }

// GameManager &GameManager::get_game_manager() { return game_manager; }

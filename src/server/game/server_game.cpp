#include "server_game.h"
#include "../../common/constant_rate_loop.h"
#include <string>

#define QUEUE_MAX_SIZE 20
#define FPS 60.0

Game::Game(const uint8_t &game_id, const World &world, const GameConfig &game_config) :
    game_id(game_id), commands(QUEUE_MAX_SIZE), config(game_config), game_manager(world) {
}

void Game::add_player(ServerSender &sender, const uint8_t &player_id) {
  // player_id = players_counter;
  // players_counter++;
  broadcaster.add_queue(&sender.get_queue(), player_id);
  player_senders.emplace_back(&sender);
  game_manager.add_player(player_id);
}

void Game::delete_player(const uint8_t &player_id) {
  broadcaster.delete_queue(player_id);
  game_manager.delete_player(player_id);
}

void Game::charge_world() {
  game_manager.initialize_game(config);
  push_game_state();
}

void Game::run() {
  try {
    std::string str =
        "[GAME-THREAD]: Game of id: " + std::to_string(static_cast<int>(game_id)) +
        " started.\n";
    std::cout << str;

    // bool was_closed = false;
    // int it = 0;
    // auto t1 = time_point_cast<milliseconds>(steady_clock::now());
    auto start_turn_time = std::chrono::steady_clock::now();

    ConstantRateLoop rate_loop(std::chrono::duration<float>(static_cast<float>(1 / FPS)),
                               [this] { return execute_frame(); });
    rate_loop.loop();

    // while (keep_playing) {
    //   // auto current_time = std::chrono::steady_clock::now();
    //   // auto elapsed_turn_time = std::chrono::duration_cast<std::chrono::seconds>(
    //   //                              current_time - start_turn_time)
    //   //                              .count();
    //   // if (elapsed_turn_time >= 60) {
    //   //   if (current_turn_id == players_counter - 1) {
    //   //     current_turn_id = 0;
    //   //   } else {
    //   //     current_turn_id++;
    //   //   }
    //   //   game_manager.set_current_turn_id(current_turn_id);
    //   //   start_turn_time = std::chrono::steady_clock::now();
    //   // }

    //   update();
    //   game_manager.update();
    //   game_manager.step();
    //   push_game_state();

    //   check_game_finished();

    //   auto t2 = time_point_cast<milliseconds>(steady_clock::now());

    //   auto time_func = duration_cast<duration<float>>(t2 - t1);
    //   auto rest = rate.count() - time_func.count();

    //   if (rest < 0) {
    //     auto behind = -rest;
    //     rest = rate.count() - fmod(behind, rate.count());

    //     auto lost = behind + rest;
    //     auto lost_ms = duration_cast<milliseconds>(dur_f(lost));

    //     t1 += lost_ms;
    //     it += floor(lost / rate.count());
    //   }

    //   auto rate_ms = duration_cast<milliseconds>(dur_f(rate));
    //   auto sleep_ms = duration_cast<milliseconds>(dur_f(rest));

    //   std::this_thread::sleep_for(sleep_ms);

    //   t1 += rate_ms;
    //   it += 1;
    // }
    std::cout << "[GAME-THREAD]: Game of id: " << static_cast<int>(game_id) << " ended."
              << std::endl;
  } catch (const std::exception &err) {
  }
}

bool Game::execute_frame() {
  if (keep_playing) {
    update();
    game_manager.update();
    game_manager.step();
    push_game_state();
    check_game_finished();
    return false;
  }
  return true;
}

void Game::update() {
  std::shared_ptr<RunnableCommandGame> runnable_command;
  while (commands.try_pop(runnable_command)) {
    runnable_command->run(game_manager);
    // it--;
  }
}

void Game::send_info_to_start_to_players() {
  for (auto &sender : player_senders) {
    sender->send_game_started();
    sender->send_world(game_manager.get_world());
  }
}

void Game::stop_playing() { keep_playing = false; }

bool Game::compare_id(const uint8_t &another_game_id) {
  return (game_id == another_game_id);
}

// void Game::set_world(World& world) {
//   game_manager.set_world(world);
// }

World *Game::get_world() { return game_manager.get_world(); }

void Game::push_game_state() {
  GameState game_state = game_manager.create_state();
  broadcaster.broadcast(game_state);
}

void Game::check_game_finished() {
  if (game_manager.is_game_finished()) {
    stop_playing();
  }
}

void Game::turn_to_started() { started = true; }

bool Game::is_started() { return started; }

bool Game::is_dead() { return !keep_playing; }

uint8_t Game::get_game_id() { return game_id; }

Queue<game_command_ptr> *Game::get_commands_queue() { return &commands; }

GameManager &Game::get_game_manager() { return game_manager; }

#include "server_game.h"

#define QUEUE_MAX_SIZE 20

Game::Game(uint8_t &game_id) : game_id(game_id), commands(QUEUE_MAX_SIZE) {
  game_manager.initialize_game();
}

Queue<std::shared_ptr<RunnableCommandGame>> *
Game::add_player(std::shared_ptr<Queue<GameState>> sender_queue,
                uint8_t &player_id) {
  last_player_id++;
  player_id = last_player_id;
  broadcaster.add_queue(sender_queue, player_id);
  game_manager.add_player(player_id);
  push_game_state();
  return &commands;
}

void Game::delete_player(const uint8_t &player_id) {
  broadcaster.delete_queue(player_id);
  game_manager.delete_player(player_id);
}

void Game::run() {
  try {
	started = true;
    bool was_closed = false;
    int it = 0;
    auto t1 = time_point_cast<milliseconds>(steady_clock::now());
    auto start_turn_time = std::chrono::steady_clock::now();
    
    while (keep_playing) {
      auto current_time = std::chrono::steady_clock::now();
      auto elapsed_turn_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_turn_time).count();
      if (elapsed_turn_time >= 60) {
        if (current_turn_id == last_player_id) {
          current_turn_id = 1;
        } else {
          current_turn_id++;
        }
        game_manager.set_current_turn_id(current_turn_id);
        start_turn_time = std::chrono::steady_clock::now();
      }

      update(it);
      game_manager.update();
      game_manager.step();

      push_game_state();

      auto t2 = time_point_cast<milliseconds>(steady_clock::now());

      auto time_func = duration_cast<duration<float>>(t2 - t1);
      auto rest = rate.count() - time_func.count();

      if (rest < 0) {
        auto behind = -rest;
        rest = rate.count() - fmod(behind, rate.count());

        auto lost = behind + rest;
        auto lost_ms = duration_cast<milliseconds>(dur_f(lost));

        t1 += lost_ms;
        it += floor(lost / rate.count());
      }

      auto rate_ms = duration_cast<milliseconds>(dur_f(rate));
      auto sleep_ms = duration_cast<milliseconds>(dur_f(rest));

      std::this_thread::sleep_for(sleep_ms);

      t1 += rate_ms;
      it += 1;
    }
  } catch (const std::exception &err) {
  }
}

void Game::update(int &it) {
  std::shared_ptr<RunnableCommandGame> runnable_command;
  while (commands.try_pop(runnable_command)) {
    runnable_command->run(game_manager);
    // it--;
  }
}

void Game::stop() { keep_playing = false; }

bool Game::compare_id(const uint8_t &another_game_id) {
  return (game_id == another_game_id);
}

void Game::push_game_state() {
  GameState game_state = game_manager.get_state();
  broadcaster.broadcast(game_state);
}

bool Game::is_started() { return started; }

bool Game::is_dead() { return not keep_playing; }

uint8_t Game::get_game_id() { return game_id; }

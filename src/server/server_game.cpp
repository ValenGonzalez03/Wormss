#include "server_game.h"
#include "math.h"
#include <chrono>
#include <unistd.h>

#define QUEUE_MAX_SIZE 20

Game::Game(int &game_id) : game_id(game_id), commands(QUEUE_MAX_SIZE) {
  game_manager.initialize_game();
}

Queue<std::shared_ptr<Command>> *Game::add_player(std::shared_ptr<Queue<GameState>> sender_queue, 
                                   const int &player_id) {
  broadcaster.add_queue(sender_queue, player_id);
  game_manager.add_player(player_id);

  push_game_state();
  return &commands;
}

void Game::delete_player(const int &player_id) {
  broadcaster.delete_queue(player_id);
  game_manager.delete_player(player_id);
}

void Game::run() {
  try {
	auto t1 = time_point_cast<milliseconds>(steady_clock::now());
    int it = 0;
    bool was_closed = false;
    while (keep_playing) {
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

void Game::update(int& it) {
  std::shared_ptr<Command> command;
  while (commands.try_pop(command)) {
	  command->run(game_manager);
	  //it--;
  }
}

void Game::stop() { keep_playing = false; }

bool Game::compare_id(const int &another_game_id) {
  return (game_id == another_game_id);
}

void Game::push_game_state() {
  GameState game_state = game_manager.get_state();
  broadcaster.broadcast(game_state);
}

bool Game::is_dead() { return not keep_playing; }

int Game::get_game_id() {
	return game_id;
}

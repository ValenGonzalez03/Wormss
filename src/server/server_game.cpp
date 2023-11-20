#include "server_game.h"
#include "math.h"
#include <chrono>
#include <unistd.h>

#define QUEUE_MAX_SIZE 20

Game::Game(int &game_id) : game_id(game_id), commands(QUEUE_MAX_SIZE) {
  game_manager.initialize_game();
}

Queue<std::shared_ptr<Command>> *Game::add_player(std::shared_ptr<Queue<GameState>> sender_queue, 
                                   int &player_id) {
  player_id = last_player_id_added + 1;
  broadcaster.add_queue(sender_queue, player_id);
  game_manager.add_player(player_id);
  last_player_id_added = player_id;
  push_game_state();
  return &commands;
}

void Game::delete_player(const int &player_id) {
  broadcaster.delete_queue(player_id);
  game_manager.delete_player(player_id);
}

void Game::run() {
  try {
	
	//while(queues_sender.size() != MAX_PLAYERS) {}  
	  
    auto time_start = std::chrono::high_resolution_clock::now();
    int it = 0;
	started = true;
    while (keep_playing) {
      update(it);
      game_manager.step();

      push_game_state();

      auto time_end = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> duration = time_end - time_start;
      double elapsed = duration.count();
      auto rest = rate - elapsed;

      if (rest < 0) {
        double behind = -rest;
        double lost = behind - fmod(behind, rate);
        time_start += std::chrono::duration_cast<
            std::chrono::high_resolution_clock::duration>(
            std::chrono::duration<double>(lost));
        it += int(lost / rate);
      } else {
        sleep(rest);
      }
      time_start += std::chrono::duration_cast<
          std::chrono::high_resolution_clock::duration>(
          std::chrono::duration<double>(rate));
      it += 1;

    }
  } catch (const std::exception &err) {
  }
}

void Game::update(int& it) {
  std::shared_ptr<Command> command;
  while (commands.try_pop(command)) {
	  command->run(game_manager);
	  it--;
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

bool Game::is_started() { return started; }

bool Game::is_dead() { return not keep_playing; }

int Game::get_game_id() { return game_id; }

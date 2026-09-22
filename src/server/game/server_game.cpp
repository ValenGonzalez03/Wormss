#include "server_game.h"
#include "../world/trajectory_missile_callback.h"
#include "../world/baseball_bat_callback.h"
#include "box2d/box2d.h"
#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>

const float TIME_STEP = (1.0f / FPS);
const int32 VEL_ITERATIONS = 6;
const int32 POS_ITERATIONS = 2;

Game::Game(const World& world) : world(world), turn_manager(&players) {}

void Game::charge_world(const GameConfig& game_config) {
  int i = 0;
  int worm_id_counter = 0;
  std::vector<std::vector<float>> spawn_points = world.get_spawn_points();
  for (auto& [_, player] : players) {
    int num_sp = i;
    if (i >= spawn_points.size()) {
      num_sp = spawn_points.size() - 1;
    }
    auto worm =
        world.create_worm(worm_id_counter++, player.get_player_id(),
                          spawn_points[num_sp][0], spawn_points[num_sp][1], game_config);
    player.add_worm(worm);
    auto string = "[CLIENT-MAN-THREAD]: Worm of id: " +
                  std::to_string(static_cast<int>(worm_id_counter - 1)) + " created.\n";
    std::cout << string;
    i++;
  }
  world.create_water();

  turn_manager.assign_first_player();
}

void Game::add_player(const uint8_t& player_id) {
  Player player(player_id);
  players.emplace(player_id, player);
  // players[player_id] = player;
}

void Game::delete_player(const uint8_t& player_id) { players.erase(player_id); }

World* Game::get_world() { return &world; }

void Game::update() {
  world.update_worms();
  world.update_explodables();
  world.update_explosions();

  update_turn();

  world.step(TIME_STEP, VEL_ITERATIONS, POS_ITERATIONS);
}

void Game::update_turn() {
  auto now = std::chrono::steady_clock::now();
  float dt = std::chrono::duration<float>(now - last_tick).count();
  last_tick = now;

  turn_manager.update(dt);
}

void Game::move(const uint8_t& player_id, const uint8_t& direction) {
  if (!turn_manager.is_turn_of(player_id)) {
    return;
  }
  auto player = turn_manager.get_current_player();
  WormBody* worm = player.get_current_worm();

  worm->start_moving(direction);
}

void Game::stop_moving(const uint8_t& player_id) {
  if (!turn_manager.is_turn_of(player_id)) {
    return;
  }
  auto player = turn_manager.get_current_player();
  WormBody* worm = player.get_current_worm();

  worm->stop_moving();
}

void Game::jump(const uint8_t& player_id, const uint8_t& direction,
                const uint8_t& jump_type) {
  if (!turn_manager.is_turn_of(player_id)) {
    return;
  }
  auto player = turn_manager.get_current_player();
  WormBody* worm = player.get_current_worm();

  worm->jump(direction, jump_type);
}

void Game::aim(const uint8_t& player_id, const uint8_t& direction) {
  if (!turn_manager.is_turn_of(player_id)) {
    return;
  }
  auto player = turn_manager.get_current_player();
  WormBody* worm = player.get_current_worm();

  worm->start_aiming(direction);
}

void Game::stop_aiming(const uint8_t& player_id) {
  if (!turn_manager.is_turn_of(player_id)) {
    return;
  }
  auto player = turn_manager.get_current_player();
  WormBody* worm = player.get_current_worm();

  worm->stop_aiming();
}

void Game::change_weapon(const uint8_t& player_id, const uint8_t& weapon_type) {
  if (!turn_manager.is_turn_of(player_id)) {
    return;
  }
  auto player = turn_manager.get_current_player();
  WormBody* worm = player.get_current_worm();

  worm->change_weapon(static_cast<WeaponType>(weapon_type));
}

void Game::set_worm_to_charge(const uint8_t& player_id) {
  if (!turn_manager.is_turn_of(player_id)) {
    return;
  }
  auto player = turn_manager.get_current_player();
  WormBody* worm = player.get_current_worm();

  worm->set_to_charge();
}

void Game::attack(const uint8_t& player_id, float charge_intensity) {
  if (!turn_manager.is_turn_of(player_id)) {
    return;
  }
  auto player = turn_manager.get_current_player();
  WormBody* worm = player.get_current_worm();

  if (worm->get_state() == ATTACKING)
    return;

  ServerWeapon* weapon = worm->get_weapon_selected();
  weapon->attack(world, charge_intensity, projectiles_id_counter);

  worm->set_to_attack();
}

GameState Game::create_state() {
  GameState game_state;
  if (game_finished) {
    game_state.game_finished = true;
    // game_state.set_game_finished();
  }

  if (turn_manager.are_players_remaining()) {
    Player current_player = turn_manager.get_current_player();
    game_state.current_turn_player_id = current_player.get_player_id();
    game_state.current_turn_worm_id = current_player.get_current_worm()->get_id();
    game_state.turn_time_remaining = turn_manager.get_time_remaining();
  }

  auto worms_attr = world.get_worms_attr();
  for (const auto& attr : worms_attr) {
    game_state.add_worm(attr);
  }

  auto explodables_attr = world.get_explodables_attr();
  for (const auto& attr : explodables_attr) {
    game_state.add_explodable(attr);
  }

  auto explosions_attr = world.get_explosions_attr();
  for (const auto& attr : explosions_attr) {
    game_state.add_explosion(attr);
  }

  return game_state;
}

void Game::set_game_finished(const bool is_finished) { game_finished = is_finished; }

bool Game::is_game_finished() const { return game_finished; }

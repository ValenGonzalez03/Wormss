#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include "../../common/game_state.h"
#include "box2d/box2d.h"
#include "../world/server_world.h"
#include "server_player.h"
#include "worlds_reader.h"
#include "game_config.h"
#include "turn_manager.h"
#include <stdio.h>
#include <map>
#include <utility>

#define FPS 60.0


class Game {
 private:
  World world;
  bool game_finished = false;
  int projectiles_id_counter = 0;
  std::map<uint8_t, Player> players = {};
  TurnManager turn_manager;
  std::chrono::steady_clock::time_point last_tick = std::chrono::steady_clock::now();

  // Actualiza el tiempo restante del turno y gestiona el cambio de turno si es necesario.
  void update_turn();

 public:
  explicit Game(const World &world);

  // Recibe un GameConfig, crea los gusanos de cada jugador en el mundo, el agua,
  // y asigna a un jugador como el jugador actual.
  void charge_world(const GameConfig &game_config);

  // Añade un jugador al juego y lo agrega a la rotación de turnos.
  void add_player(const uint8_t &player_id);

  // Elimina un jugador del juego y de la rotación de turnos.
  void delete_player(const uint8_t &player_id);

  World *get_world();

  // Actualiza el estado de los gusanos, proyectiles y explosiones en el mundo,
  // gestiona el turno actual, y avanza al siguiente frame en el mundo.
  void update();

  // Recibe una direction y gestiona el comienzo del movimiento del gusano actual
  // del jugador en la dirección especificada.
  void move(const uint8_t &player_id, const uint8_t &direction);

  // Gestiona la detención del movimiento del gusano actual del jugador.
  void stop_moving(const uint8_t &player_id);

  // Recibe una direction y un jump_type, y gestiona el salto del gusano actual
  // del jugador en la dirección y tipo de salto especificados.
  void jump(const uint8_t &player_id, const uint8_t &direction, const uint8_t &jump_type);

  // Recibe una direction y gestiona el comienzo del apuntado del gusano actual del
  // jugador en la dirección especificada.
  void aim(const uint8_t &player_id, const uint8_t &direction);

  // Gestiona la detención del apuntado del gusano actual del jugador.
  void stop_aiming(const uint8_t &player_id);

  // Recibe un weapon_type y gestiona el cambio de arma del gusano actual del jugador
  // al arma especificada.
  void change_weapon(const uint8_t &player_id, const uint8_t &weapon_type);

  // Gestiona el comienzo de la carga del ataque del gusano actual del jugador.
  void set_worm_to_charge(const uint8_t &player_id);

  // Recibe un charge_intensity y gestiona el ataque del gusano actual del jugador
  // con la intensidad de carga especificada.
  void attack(const uint8_t &player_id, float charge_intensity);

  // Carga toda la informacion requerida por el cliente en un GameState y lo devuelve.
  GameState create_state();

  void set_game_finished(const bool is_finished);

  bool is_game_finished() const;

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;
};

#endif

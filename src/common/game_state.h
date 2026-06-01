#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <arpa/inet.h>
#include <cstdint>
#include <list>
#include <map>
#include <vector>
#include <utility>

#include "position.h"
#include "game_constants.h"
#include "../client/comm/client_protocol.h"
#include "../server/comm/server_protocol.h"


struct WormData {
 private:
  uint8_t player_id;
  float pos_x;  // En metros
  float pos_y;  // En metros
  uint8_t direction;
  WormState state;
  WeaponType current_weapon;
  float aim_angle;

 public:
  // Default constructor (PARA QUE COMPILE, REVISAR!!!!)
  WormData() :
      player_id(-1), pos_x(0), pos_y(0), direction(RIGHT), state(IDLE),
      current_weapon(BAZOOKA), aim_angle(0) {}

  explicit WormData(uint8_t id, float pos_x, float pos_y, u_int8_t dir, WormState st,
                    WeaponType wp, float angle) :
      player_id(id), pos_x(pos_x), pos_y(pos_y), direction(dir), state(st),
      current_weapon(wp), aim_angle(angle) {}

  explicit WormData(ClientProtocol &prot) : pos_x(0), pos_y(0) {
    bool was_closed = false;
    deserialize(prot, &was_closed);
  }

  // Recibe la pos, la direccion, el state, etc del gusano (Lado cliente)
  void deserialize(ClientProtocol &prot,  // NOLINT(runtime/references)
                   bool *was_closed) {
    // Recibo el player_id
    this->player_id = prot.recv_byte(was_closed);

    // Recibo la position
    this->pos_x = prot.recv_float(was_closed);
    this->pos_y = prot.recv_float(was_closed);

    // Recibo la direccion
    this->direction = prot.recv_byte(was_closed);

    // Recibo el estado
    this->state = static_cast<WormState>(prot.recv_byte(was_closed));

    // Recibo el arma actual equipada
    this->current_weapon = static_cast<WeaponType>(prot.recv_byte(was_closed));

    // Recibo el angulo de apuntado
    this->aim_angle = prot.recv_float(was_closed);
  }

  // Envia los datos del gusano (Lado servidor)
  void serialize(ServerProtocol &prot, bool *was_closed) {  // NOLINT(runtime/references)
    // Envio el player_id
    prot.send_byte(this->player_id, was_closed);

    // Envio la posicion
    prot.send_float(pos_x, was_closed);
    prot.send_float(pos_y, was_closed);

    // Envio la direccion
    prot.send_byte(
        this->direction,
        was_closed);  // Si bien pone send_id, es un uint8_t. Luego debo cambiarle el nombre a la funcion

    // Envio el estado
    prot.send_byte(this->state, was_closed);

    // Envio el arma actual equipada
    prot.send_byte(this->current_weapon, was_closed);

    // Envio el angulo de apuntado
    prot.send_float(this->aim_angle, was_closed);
  }

  uint8_t get_player_id() const { return player_id; }

  float get_pos_x() const { return pos_x; }

  float get_pos_y() const { return pos_y; }

  uint8_t get_direction() const { return direction; }

  WormState get_state() const { return state; }

  WeaponType get_weapon_selected() const { return current_weapon; }

  float get_aim_angle() const { return aim_angle; }
};

struct ExplodableData {
 private:
  float pos_x;  // En metros
  float pos_y;  // En metros
  float angle;  // En radianes
  BODY_TYPES type;
  uint8_t direction;
  uint8_t id;

 public:
  // Default constructor (PARA QUE COMPILE, REVISAR!!!!)
  ExplodableData() : pos_x(0), pos_y(0), angle(0), type(WATER), direction(0), id(0) {}

  explicit ExplodableData(float pos_x, float pos_y, float angle, BODY_TYPES type,
                          uint8_t dir, uint8_t id) :
      pos_x(pos_x), pos_y(pos_y), angle(angle), type(type), direction(dir), id(id) {}

  explicit ExplodableData(ClientProtocol &prot) :  // NOLINT(runtime/references)
      pos_x(0), pos_y(0), angle(0), type(WATER), direction(0), id(0) {
    bool was_closed = false;
    deserialize(prot, &was_closed);
  }

  void deserialize(ClientProtocol &prot,  // NOLINT(runtime/references)
                   bool *was_closed) {
    // Recibo el id del proyectil
    this->id = prot.recv_byte(was_closed);

    // Recibo el tipo de proyectil
    this->type = static_cast<BODY_TYPES>(prot.recv_byte(was_closed));

    // Recibo la posicion del proyectil
    this->pos_x = prot.recv_float(was_closed);
    this->pos_y = prot.recv_float(was_closed);

    // Recibo el angulo del proyectil
    this->angle = prot.recv_float(was_closed);

    // Recibo la direccion del proyectil
    this->direction = prot.recv_byte(was_closed);
  }

  void serialize(ServerProtocol &prot, bool *was_closed) {  // NOLINT(runtime/references)
    // Envio el id del proyectil
    prot.send_byte(id, was_closed);

    // Envio el tipo de proyectil
    prot.send_byte(static_cast<uint8_t>(type), was_closed);

    // Envio la posicion del proyectil
    prot.send_float(pos_x, was_closed);
    prot.send_float(pos_y, was_closed);

    // Envio el angulo del proyectil
    prot.send_float(angle, was_closed);

    // Envio la direccion del proyectil
    prot.send_byte(direction, was_closed);
  }

  float get_pos_x() { return pos_x; }

  float get_pos_y() { return pos_y; }

  float get_angle() { return angle; }

  BODY_TYPES get_type() { return type; }

  uint8_t get_direction() { return direction; }

  uint8_t get_id() { return id; }
};

struct ExplosionData {
 private:
  float pos_x;   // En metros
  float pos_y;   // En metros
  float radius;  // En radianes
  std::vector<float> rays_fraction = std::vector<float>(NUM_RAYS);

 public:
  // Default constructor (PARA QUE COMPILE, REVISAR!!!!)
  ExplosionData() : pos_x(0), pos_y(0), radius(0) {}

  explicit ExplosionData(float pos_x, float pos_y, float radius,
                         const std::vector<float> &rays) :
      pos_x(pos_x), pos_y(pos_y), radius(radius), rays_fraction(rays) {}

  explicit ExplosionData(ClientProtocol &prot) : pos_x(0), pos_y(0), radius(0) {
    bool was_closed = false;
    deserialize(prot, &was_closed);
  }

  void deserialize(ClientProtocol &prot,  // NOLINT(runtime/references)
                   bool *was_closed) {
    // Recibo la position
    this->pos_x = prot.recv_float(was_closed);
    this->pos_y = prot.recv_float(was_closed);

    // Recibo el radio de la explosion
    this->radius = prot.recv_float(was_closed);

    // Recibo las fraciones de cada rayo de la explosion
    for (int i = 0; i < NUM_RAYS; i++) {
      float final_ray = prot.recv_float(was_closed);
      rays_fraction[i] = final_ray;
    }
  }

  void serialize(ServerProtocol &prot, bool *was_closed) {  // NOLINT(runtime/references)
    // Envio la posicion
    prot.send_float(pos_x, was_closed);
    prot.send_float(pos_y, was_closed);

    // Envio el radio de la explosion
    prot.send_float(radius, was_closed);

    // Envio las fraciones de cada rayo de la explosion
    for (int i = 0; i < NUM_RAYS; i++) {
      prot.send_float(rays_fraction[i], was_closed);
    }
  }

  float get_pos_x() { return pos_x; }

  float get_pos_y() { return pos_y; }

  float get_radius() { return radius; }

  std::vector<float> get_rays_fraction() { return rays_fraction; }
};

struct GameState {
 private:
  bool game_finished = false;
  std::map<uint8_t, WormData> worms_list;
  std::map<uint8_t, ExplodableData> explodables_list;
  std::list<ExplosionData> explosions_list;

 public:
  GameState() : worms_list(std::map<uint8_t, WormData>()) {}

  explicit GameState(const std::map<uint8_t, WormData> &list) : worms_list(list) {}

  // Constructor que funciona como una deserializacion, recibe la tira de bytes y devuelve un game state
  GameState(ClientProtocol &prot, bool *was_closed) : worms_list() {
    uint8_t worms_amount = prot.recv_byte(was_closed);
    uint8_t explodables_amount = prot.recv_byte(was_closed);
    uint8_t explosions_amount = prot.recv_byte(was_closed);

    for (int i = 0; i < worms_amount; i++) {
      WormData worm(prot);
      worms_list.insert(std::pair<uint8_t, WormData>(worm.get_player_id(), worm));
    }
    for (int i = 0; i < explodables_amount; i++) {
      ExplodableData explodable(prot);
      explodables_list.insert(
          std::pair<uint8_t, ExplodableData>(explodable.get_id(), explodable));
    }
    for (int i = 0; i < explosions_amount; i++) {
      ExplosionData explosion(prot);
      explosions_list.push_back(explosion);
    }

    this->game_finished = static_cast<bool>(prot.recv_byte(was_closed));
  }

  void serialize(ServerProtocol &prot, bool *was_closed) {  // NOLINT(runtime/references)
    prot.send_byte(worms_list.size(), was_closed);
    prot.send_byte(explodables_list.size(), was_closed);
    prot.send_byte(explosions_list.size(), was_closed);

    for (auto &worm : worms_list) {
      worm.second.serialize(prot, was_closed);
    }
    for (auto &explodable : explodables_list) {
      explodable.second.serialize(prot, was_closed);
    }
    for (auto &explosion : explosions_list) {
      explosion.serialize(prot, was_closed);
    }

    uint8_t finished = static_cast<uint8_t>(this->game_finished);
    prot.send_byte(finished, was_closed);
  }

  std::map<uint8_t, WormData> get_worms() const { return worms_list; }

  void add_worm(const WormAttr &attr) {
    WormData worm(attr.player_id, attr.pos_x, attr.pos_y, attr.direction, attr.state,
                  attr.current_weapon, attr.aim_angle);
    worms_list.insert(std::pair<uint8_t, WormData>(worm.get_player_id(), worm));
  }

  std::map<uint8_t, ExplodableData> get_explodables() const { return explodables_list; }

  void add_explodable(const ExplodableAttr &attr) {
    ExplodableData explodable(attr.pos_x, attr.pos_y, attr.angle, attr.type,
                              attr.direction, attr.id);
    explodables_list.insert(
        std::pair<uint8_t, ExplodableData>(explodable.get_id(), explodable));
  }

  std::list<ExplosionData> get_explosions() const { return explosions_list; }

  void add_explosion(const ExplosionAttr &attr) {
    ExplosionData explosion(attr.pos_x, attr.pos_y, attr.radius, attr.ray_fractions);
    explosions_list.push_back(explosion);
  }

  bool is_game_finished() const { return game_finished; }

  void set_game_finished() { game_finished = true; }
};

#endif

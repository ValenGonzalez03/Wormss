#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <arpa/inet.h>
#include <cstdint>
#include <list>
#include <map>
#include <vector>

#include "position.h"
#include "game_constants.h"
#include "lib/socket.h"


struct WormData {
private:
  uint8_t player_id;
  float pos_x; // En metros
  float pos_y; // En metros
  uint8_t direction;
  WormState state;
  float aim_angle;

public:
  // Default constructor (PARA QUE COMPILE, REVISAR!!!!)
  explicit WormData()
      : player_id(-1), pos_x(0), pos_y(0), direction(RIGHT), state(IDLE), aim_angle(0) {}

  explicit WormData(uint8_t id, float pos_x, float pos_y, u_int8_t dir, WormState st, float angle)
      : player_id(id), pos_x(pos_x), pos_y(pos_y), direction(dir), state(st), aim_angle(angle) {}

  explicit WormData(Socket &skt) : pos_x(0), pos_y(0) {
    bool was_closed = false;
    deserialize(skt, &was_closed);
  }

  // Recibe la pos, la direccion, el state, etc del gusano (Lado cliente)
  void deserialize(Socket &skt, bool *was_closed) {
    // Recibo el player_id
    skt.recvall(&player_id, sizeof(player_id), was_closed);
  
    // Recibo la position
    uint16_t pos_x;
    uint16_t pos_y;
    skt.recvall(&pos_x, sizeof(pos_x), was_closed);
    skt.recvall(&pos_y, sizeof(pos_y), was_closed);
    float final_pos_x = ntohs(pos_x) / 100.0;
    float final_pos_y = ntohs(pos_y) / 100.0;
    // std::cout << "final_pos_x: " << final_pos_x << std::endl;
    // std::cout << "final_pos_y: " << final_pos_y << std::endl;
    this->pos_x = final_pos_x;
    this->pos_y = final_pos_y;
  
    // Recibo la direccion
    skt.recvall(&(this->direction), sizeof(this->direction), was_closed);
  
    // Recibo el estado
    skt.recvall(&(this->state), sizeof(this->state), was_closed);
  
    // Recibo el angulo de apuntado
    int angle_int_net;
    skt.recvall(&angle_int_net, sizeof(angle_int_net), was_closed);
    int angle_int = ntohl(angle_int_net);
    this->aim_angle = float(angle_int) / float(100.0);
  }

  // Envia los datos del gusano (Lado servidor)
  void serialize(Socket &skt, bool *was_closed) {
    // Envio el player_id
    skt.sendall(&(this->player_id), sizeof(this->player_id), was_closed);
  
    // Envio la posicion
    // std::cout << "final_pos_x: " << pos_x << std::endl;
    // std::cout << "final_pos_y: " << pos_y << std::endl;
    uint16_t pos_x = uint(this->pos_x * 100);
    uint16_t pos_y = uint(this->pos_y * 100);
    uint16_t pos_x_be = htons(pos_x);
    uint16_t pos_y_be = htons(pos_y);
    skt.sendall(&pos_x_be, sizeof(pos_x_be), was_closed);
    skt.sendall(&pos_y_be, sizeof(pos_y_be), was_closed);
  
    // Envio la direccion
    skt.sendall(&(this->direction), sizeof(this->direction), was_closed);
  
    // Envio el estado
    skt.sendall(&(this->state), sizeof(this->state), was_closed);
  
    // Envio el angulo de apuntado
    int angle_int = int(this->aim_angle * 100);
    int angle_int_net = htonl(angle_int);
    skt.sendall(&angle_int_net, sizeof(angle_int_net), was_closed);
  }

  uint8_t get_player_id() { return player_id; }

  float get_pos_x() { return pos_x; }

  float get_pos_y() { return pos_y; }

  uint8_t get_direction() { return direction; }

  WormState get_state() { return state; }

  float get_aim_angle() { return aim_angle; }

  // Devuelve la pos del gusano. Uso const para evitar que sea modificada
  //Position get_position() const { return pos; }
};

struct MissileData {
private:
  float pos_x; // En metros
  float pos_y; // En metros
  float angle; // En radianes
  uint8_t direction;
  uint8_t missile_id;

public:

  // Default constructor (PARA QUE COMPILE, REVISAR!!!!)
  explicit MissileData() : pos_x(0), pos_y(0), angle(0), direction(0), missile_id(0) {}

  explicit MissileData(float pos_x, float pos_y, float angle, uint8_t dir, uint8_t id)
      : pos_x(pos_x), pos_y(pos_y), angle(angle), direction(dir), missile_id(id) {}

  explicit MissileData(Socket &skt) : pos_x(0), pos_y(0), angle(0), direction(0), missile_id(0) {
    bool was_closed = false;
    deserialize(skt, &was_closed);
  }

  void deserialize(Socket &skt, bool *was_closed) {
    // Recibo el id del misil
    skt.recvall(&missile_id, sizeof(missile_id), was_closed);

    // Recibo la position
    uint16_t pos_x;
    uint16_t pos_y;
    skt.recvall(&pos_x, sizeof(pos_x), was_closed);
    skt.recvall(&pos_y, sizeof(pos_y), was_closed);
    float final_pos_x = ntohs(pos_x) / 100.0;
    float final_pos_y = ntohs(pos_y) / 100.0;
    // std::cout << "final_pos_x: " << final_pos_x << std::endl;
    // std::cout << "final_pos_y: " << final_pos_y << std::endl;
    this->pos_x = final_pos_x;
    this->pos_y = final_pos_y;
  
    // Recibo el angulo del misil
    int angle_int_net;
    skt.recvall(&angle_int_net, sizeof(angle_int_net), was_closed);
    int angle_int = ntohl(angle_int_net);
    this->angle = float(angle_int) / float(100.0);

    // Recibo la direccion del misil
    skt.recvall(&direction, sizeof(direction), was_closed);
  }

  void serialize(Socket &skt, bool *was_closed) {
    // Envio el id del misil
    skt.sendall(&(missile_id), sizeof(missile_id), was_closed);

    // Envio la posicion
    uint16_t pos_x = uint(this->pos_x * 100);
    uint16_t pos_y = uint(this->pos_y * 100);
    uint16_t pos_x_be = htons(pos_x);
    uint16_t pos_y_be = htons(pos_y);
    skt.sendall(&pos_x_be, sizeof(pos_x_be), was_closed);
    skt.sendall(&pos_y_be, sizeof(pos_y_be), was_closed);

    // Envio el angulo del misil
    int angle_int = int(this->angle * 100);
    int angle_int_net = htonl(angle_int);
    skt.sendall(&angle_int_net, sizeof(angle_int_net), was_closed);

    // Envio la direccion del misil
    skt.sendall(&direction, sizeof(direction), was_closed);
  }

  float get_pos_x() { return pos_x; }

  float get_pos_y() { return pos_y; }

  float get_angle() { return angle; }

  uint8_t get_direction() { return direction; }

  uint8_t get_id() { return missile_id; }
};

struct ExplosionData {
  private:
    float pos_x; // En metros
    float pos_y; // En metros
    float radius; // En radianes
    std::vector<float> rays_fraction = std::vector<float> (NUM_RAYS);
  
  public:
  
    // Default constructor (PARA QUE COMPILE, REVISAR!!!!)
    explicit ExplosionData() : pos_x(0), pos_y(0), radius(0) {}
  
    explicit ExplosionData(float pos_x, float pos_y, float radius, std::vector<float> rays)
        : pos_x(pos_x), pos_y(pos_y), radius(radius), rays_fraction(rays) {}
  
    explicit ExplosionData(Socket &skt) : pos_x(0), pos_y(0), radius(0) {
      bool was_closed = false;
      deserialize(skt, &was_closed);
    }
  
    void deserialize(Socket &skt, bool *was_closed) {
      // Recibo la position
      uint16_t pos_x;
      uint16_t pos_y;
      skt.recvall(&pos_x, sizeof(pos_x), was_closed);
      skt.recvall(&pos_y, sizeof(pos_y), was_closed);
      float final_pos_x = ntohs(pos_x) / 100.0;
      float final_pos_y = ntohs(pos_y) / 100.0;
      this->pos_x = final_pos_x;
      this->pos_y = final_pos_y;
    
      // Recibo el radio de la explosion
      uint16_t radius;
      skt.recvall(&radius, sizeof(radius), was_closed);
      float final_radius = ntohs(radius) / 100.0;
      this->radius = final_radius;
  
      for (int i = 0 ; i < NUM_RAYS ; i++) {
        uint16_t ray;
        skt.recvall(&ray, sizeof(ray), was_closed);
        float final_ray = ntohs(ray) / 100.0;
        rays_fraction.assign(i, final_ray);
      }
    }
  
    void serialize(Socket &skt, bool *was_closed) {    
      // Envio la posicion
      uint16_t pos_x = uint(this->pos_x * 100);
      uint16_t pos_y = uint(this->pos_y * 100);
      uint16_t pos_x_be = htons(pos_x);
      uint16_t pos_y_be = htons(pos_y);
      skt.sendall(&pos_x_be, sizeof(pos_x_be), was_closed);
      skt.sendall(&pos_y_be, sizeof(pos_y_be), was_closed);
      
      // Envio el radio de la explosion
      uint16_t radius = uint(this->radius * 100);
      uint16_t radius_be = htons(radius);
      skt.sendall(&radius_be, sizeof(radius_be), was_closed);
      
      for (int i = 0 ; i < NUM_RAYS ; i++) {
        uint16_t ray = uint(rays_fraction[i] * 100);
        uint16_t ray_be = htons(pos_x);
        skt.sendall(&ray_be, sizeof(ray_be), was_closed);
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
  std::map<uint8_t, MissileData> missiles_list;
  std::list<ExplosionData> explosions_list;

public:
  GameState() : worms_list(std::map<uint8_t, WormData>()) {}

  explicit GameState(const std::map<uint8_t, WormData> &list) : worms_list(list) {}

  // Constructor que funciona como una deserializacion, recibe la tira de bytes
  // y devuelve un game state
  GameState(Socket &skt, bool *was_closed) : worms_list() {

    uint8_t worms_amount = 0;
    uint8_t missiles_amount = 0;
    uint8_t explosions_amount = 0;
    skt.recvall(&worms_amount, sizeof(worms_amount), was_closed);
    skt.recvall(&missiles_amount, sizeof(missiles_amount), was_closed);
    skt.recvall(&explosions_amount, sizeof(explosions_amount), was_closed);
    
    for (int i = 0; i < worms_amount; i++) {
      WormData worm(skt);
      worms_list.insert(std::pair<uint8_t, WormData>(worm.get_player_id(), worm));
    }
    for (int i = 0; i < missiles_amount; i++) {
      MissileData missile(skt);
      missiles_list.insert(std::pair<uint8_t, MissileData>(missile.get_id(), missile));
    }
    for (int i = 0; i < explosions_amount; i++) {
      ExplosionData explosion(skt);
      explosions_list.push_back(explosion);
    }
    
    bool game_finished;
    skt.recvall(&game_finished, sizeof(game_finished), was_closed);
    this->game_finished = game_finished;
  }

  void serialize(Socket &skt, bool *was_closed) {
    uint8_t worms_amount = worms_list.size();
    uint8_t missiles_amount = missiles_list.size();
    uint8_t explosions_amount = explosions_list.size();
    skt.sendall(&worms_amount, sizeof(worms_amount), was_closed);
    skt.sendall(&missiles_amount, sizeof(missiles_amount), was_closed);
    skt.sendall(&explosions_amount, sizeof(explosions_amount), was_closed);

    for (auto &worm : worms_list) {
      worm.second.serialize(skt, was_closed);
    }
    for (auto &missile : missiles_list) {
      missile.second.serialize(skt, was_closed);
    }
    for (auto &explosion : explosions_list) {
      explosion.serialize(skt, was_closed);
    }
  
    skt.sendall(&game_finished, sizeof(game_finished), was_closed);
  }

  std::map<uint8_t, WormData> get_worms() { return worms_list; }

  void add_worm(WormAttr& attr) {
    WormData worm(attr.player_id, attr.pos_x, attr.pos_y, attr.direction, attr.state, attr.aim_angle);
    worms_list.insert(std::pair<uint8_t, WormData>(worm.get_player_id(), worm));
  }

  std::map<uint8_t, MissileData> get_missiles() { return missiles_list; }

  void add_missile(MissileAttr& attr) {
    MissileData missile(attr.pos_x, attr.pos_y, attr.angle, attr.direction, attr.missile_id);
    missiles_list.insert(std::pair<uint8_t, MissileData>(missile.get_id(), missile));
  }

  std::list<ExplosionData> get_explosions() { return explosions_list; }

  void add_explosion(ExplosionAttr& attr) {
    ExplosionData explosion (attr.pos_x, attr.pos_y, attr.radius, attr.ray_fractions);
    explosions_list.push_back(explosion);
  }

  bool is_game_finished() const { return game_finished; }

  void set_game_finished() { 
    game_finished = true; 
  }
};

#endif

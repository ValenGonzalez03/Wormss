#ifndef SERVER_WORLD_H
#define SERVER_WORLD_H

#include "box2d/box2d.h"
#include "../game/game_config.h"
#include "contact_listener.h"
#include "../bodies/worm_body.h"
#include "../bodies/beam_body.h"
#include "../bodies/missile_body.h"
#include "../bodies/grenade_body.h"
#include "server_explosion.h"
#include <list>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <string>

class World {
 private:
  std::shared_ptr<b2World> world;
  std::list<WormBody*> worms;
  std::list<BeamBody*> beams;
  std::list<Explodable*> explodables;
  std::list<Explosion> explosions;
  std::list<Body*> bodies;
  std::string name = "";
  std::string background = "";
  std::vector<std::vector<float>> spawn_points;
  ContactListener contact_listener;

  void destroy_body(Body* body);

  void delete_worms();

  void delete_beams();

  void delete_explodables();

 public:
  World();

  WormBody* create_worm(const uint8_t player_id, const float spawn_x, const float spawn_y, const GameConfig& config);

  BeamBody* create_beam(float pos_x, float pos_y, float angle, float length);

  MissileBody* create_missile(uint8_t id, float pos_x, float pos_y, float angle, uint8_t direction,
                              float initial_force);

  GrenadeBody* create_grenade(uint8_t id, float pos_x, float pos_y, float angle, uint8_t direction,
                              float initial_force);

  void create_explosion(float center_x, float center_y);

  void step(float timeStep, int32 velocityIterations, int32 positionIterations);

  WormBody* get_worm(const uint8_t& player_id);

  void update_worms();

  void update_explodables();

  void update_explosions();

  int get_worms_number();

  int get_explodables_number();

  void ray_cast(b2RayCastCallback* callback, const b2Vec2& point1, const b2Vec2& point2);

  ~World();

  std::string get_name() const { return this->name; }

  std::string get_background() const { return this->background; }

  std::list<WormAttr> get_worms_attr();

  std::list<ExplodableAttr> get_explodables_attr();

  std::list<ExplosionAttr> get_explosions_attr();

  std::list<BeamBody*> get_beams() const { return this->beams; }

  void set_name(const std::string& new_name) { this->name = new_name; }

  void set_background(const std::string& new_background) { this->background = new_background; }

  std::vector<std::vector<float>> get_spawn_points() { return spawn_points; }

  void add_spawn_point(float pos_x, float pos_y) {
    std::vector<float> spawn_point;
    spawn_point.push_back(pos_x);
    spawn_point.push_back(pos_y);
    spawn_points.push_back(spawn_point);
  }

  World(const World& other) :
      world(std::make_shared<b2World>(b2Vec2(0.0f, -10.0f))), worms(other.worms), name(other.name),
      background(other.background), contact_listener(other.contact_listener) {
    world->SetContactListener(&contact_listener);
    for (const auto& beam : other.beams) {
      create_beam(beam->get_pos_x(), beam->get_pos_y(), beam->get_angle(), beam->get_width());
    }
    for (const auto& spawn_point : other.spawn_points) {
      add_spawn_point(spawn_point[0], spawn_point[1]);
    }
  }

  World& operator=(const World& other) {
    if (this == &other) {
      return *this;
    }

    this->name = other.name;
    this->background = other.background;
    this->contact_listener = other.contact_listener;
    world->SetContactListener(&contact_listener);
    this->world = std::make_shared<b2World>(b2Vec2(0.0f, -10.0f));

    for (const auto& beam : other.beams) {
      this->create_beam(beam->get_pos_x(), beam->get_pos_y(), beam->get_angle(), beam->get_width());
    }
    for (const auto& spawn_point : other.spawn_points) {
      this->add_spawn_point(spawn_point[0], spawn_point[1]);
    }

    return *this;
  }
};

#endif

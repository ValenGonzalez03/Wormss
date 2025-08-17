#ifndef WORLD_H
#define WORLD_H

#include "box2d/box2d.h"
#include "game_config.h"
#include "contact_listener.h"
#include "bodies/worm_body.h"
#include "bodies/beam_body.h"
#include "bodies/missile_body.h"
#include <list>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <memory>

class World {
private:
  std::shared_ptr<b2World> world;
  std::list<WormBody*> worms;
  std::list<BeamBody*> beams;
  std::list<MissileBody*> missiles;
  std::list<Body*> bodies;
  std::string name = "";
  std::string background = "";
  std::vector<std::vector<float>> spawn_points;
  ContactListener contact_listener;

  void destroy_body(Body* body);

  void delete_worms();
  
  void delete_beams();
  
  void delete_missiles();

public:
  explicit World();
	
  WormBody* create_worm(const uint8_t player_id, const float spawn_x, const float spawn_y, GameConfig& config);

  BeamBody* create_beam(float pos_x, float pos_y, int angle, float length);

  MissileBody* create_missile(MissileBody* missile, float aim_angle);

  void step(float timeStep, int32 velocityIterations, int32 positionIterations);

  WormBody* get_worm(const uint8_t &player_id);

  void update_worms();

  void update_missiles();

  int get_worms_number();

  int get_missiles_number();
	
  ~World();

  std::string get_name() {
    return this->name;
  }

  std::string get_background() {
    return this->background;
  }

  std::list<WormAttr> get_worms_attr();

  std::list<MissileAttr> get_missiles_attr();

  std::list<BeamBody*> get_beams() {
    return this->beams;
  }

  void set_name(std::string new_name) {
    this->name = new_name;
  }

  void set_background(std::string new_background) {
    this->background = new_background;
  }

  std::vector<std::vector<float>> get_spawn_points() {
    return spawn_points;
  }

  void add_spawn_point(float pos_x, float pos_y) {
    std::vector<float> spawn_point;
    spawn_point.push_back(pos_x);
    spawn_point.push_back(pos_y);
    spawn_points.push_back(spawn_point);
  }
	
  World(const World& other)
        : world(std::make_shared<b2World>(b2Vec2(0.0f, -10.0f))),
          worms(other.worms),
          name(other.name),
          background(other.background),
          contact_listener(other.contact_listener) {
        world->SetContactListener(&contact_listener);
        for (const auto& beam : other.beams) {
          create_beam(beam->get_pos_x(), beam->get_pos_y(), beam->get_angle(), beam->get_width());
        }
        
        for (const auto& spawn_point : other.spawn_points) {
          add_spawn_point(spawn_point[0], spawn_point[1]);
        }
  }

  World &operator=(const World& other) {
    if (this == &other) {
        return *this;
    }

    this->world = std::make_shared<b2World>(b2Vec2(0.0f, -10.0f));
    
    for (const auto& beam : other.beams) {
      this->create_beam(beam->get_pos_x(), beam->get_pos_y(), beam->get_angle(), beam->get_width());
    }

    for (const auto& spawn_point : other.spawn_points) {
      this->add_spawn_point(spawn_point[0], spawn_point[1]);
    }

    this->name = other.name;
    this->background = other.background;
    this->contact_listener = other.contact_listener;
    world->SetContactListener(&contact_listener);

    return *this;
  }
};

#endif

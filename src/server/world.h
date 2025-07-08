#ifndef WORLD_H
#define WORLD_H

#include "bodies/beam_body.h"
#include "box2d/box2d.h"
#include "bodies/worm_body.h"
#include "bodies/missile_body.h"
#include "bodies/water_body.h"
#include "bodies/weapons/bazooka.h"
#include "bodies/weapons/banana.h"
#include "bodies/weapons/dynamite.h"
#include "contact_listener.h"
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

public:
  explicit World();
	
  BeamBody *create_beam(float pos_x, float pos_y, int angle, float length);

  WormBody* create_worm(float pos_x, float pos_y, float vel, int health, const uint8_t& player_id);
  
  void create_water(float pos_x, float pos_y, float width, int height);
  
  void create_bazooka_missile(float pos_x, float pos_y);
  
  void create_banana(float pos_x, float pos_y);
  
  void create_dynamite(float pos_x, float pos_y);
	
  void step(float timeStep, int32 velocityIterations, int32 positionIterations);
	
  void delete_worms();
	
  void delete_beams();
	
  ~World();

  std::string get_name() {
    return this->name;
  }

  std::string get_background() {
    return this->background;
  }

  std::list<WormBody*> get_worms() {
    return this->worms;
  }

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

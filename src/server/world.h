#ifndef WORLD_H
#define WORLD_H

#include "beam_body.h"
#include "box2d/box2d.h"
#include "worm_body.h"
#include "contact_listener.h"
#include <list>
#include <stdio.h>
#include <iostream>

class World {
private:
  b2World* world;
  std::list<WormBody*> worms;
  std::list<BeamBody*> beams;
  std::string name = "";
  std::string background = "";
  ContactListener contact_listener;

public:
  explicit World();
	
  BeamBody *create_beam(float pos_x, float pos_y);

	BeamBody *create_beam(float pos_x, float pos_y, int angle, float length);

  WormBody *create_worm(float pos_x, float pos_y);
	
  void step(float timeStep, int32 velocityIterations, int32 positionIterations);
	
  void delete_worms();
	
  void delete_beams();
	
  ~World();

/*
  void print_beams () {
    for (auto &beam : beams) {
      beam->print_beam();
    }
  }

  void print_worms () {
    for (auto &worm : worms) {
      std::cout << "WORM" << std::endl;
      std::cout << "pos_x: " << worm->get_pos_x() << std::endl;
      std::cout << "pos_y: " << worm->get_pos_y() << std::endl;
    }
  }
*/

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
	
  World(const World &) = delete;
  World &operator=(const World &) = delete;
};

#endif

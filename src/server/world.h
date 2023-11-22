#ifndef WORLD_H
#define WORLD_H

#include "beam_body.h"
#include "box2d/box2d.h"
#include "worm_body.h"
#include <list>
#include <stdio.h>

class World {
private:
  b2World* world;
  std::list<WormBody*> worms;
  std::list<BeamBody*> beams;

public:
  explicit World();
	
  BeamBody *create_beam(float pos_x, float pos_y);

	BeamBody *create_beam(float pos_x, float pos_y, int angle, float length);

  WormBody *create_worm(float pos_x, float pos_y);
	
  void step(float timeStep, int32 velocityIterations, int32 positionIterations);
	
  void delete_worms();
	
  void delete_beams();
	
  ~World();
	
  World(const World &) = delete;
  World &operator=(const World &) = delete;
};

#endif

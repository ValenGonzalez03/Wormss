#include "world.h"
#include "box2d/box2d.h"
#include <stdio.h>

World::World() : world(b2World(b2Vec2(0.0f, -10.0f))) {
  world.SetContactListener(&contact_listener);
}

BeamBody* World::create_beam(float pos_x, float pos_y) {
  BeamBody* beam = new BeamBody(&world, pos_x, pos_y);
  beams.push_back(beam);
  return beam;
}

BeamBody* World::create_beam(float pos_x, float pos_y, int angle, float length) {
  BeamBody* beam = new BeamBody(&world, pos_x, pos_y, angle, length);
  beams.push_back(beam);
  return beam;
}

WormBody* World::create_worm(float pos_x, float pos_y, const uint8_t& player_id) {
  WormBody* worm = new WormBody(&world, pos_x, pos_y, player_id);
  worms.push_back(worm);
  return worm;
}

void World::step(float timeStep, int32 velocityIterations, 
				 int32 positionIterations) {
  world.Step(timeStep, velocityIterations, positionIterations);
}

void World::delete_worms() {
  for (auto &worm : worms) {
	delete worm;
  }
  worms.clear();
}

void World::delete_beams() {
  for (auto &beam : beams) {
    delete beam;
  }
  beams.clear();
}

World::~World() {
  for (auto &current_worm : worms) {
    delete current_worm;
  }

  for (auto &current_beam : beams) {
    delete current_beam;
  }

  //delete world;
}


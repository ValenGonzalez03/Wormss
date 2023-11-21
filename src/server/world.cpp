#include "world.h"
#include "box2d/box2d.h"
#include <stdio.h>

World::World() {
  b2Vec2 gravity(0.0f, -10.0f);
  world = new b2World(gravity);
}

BeamBody* World::create_beam(float pos_x, float pos_y) {
  BeamBody* beam = new BeamBody(world, pos_x, pos_y);
  beams.push_back(beam);
  return beam;
}

BeamBody* World::create_beam(float pos_x, float pos_y, int angle) {
  BeamBody* beam = new BeamBody(world, pos_x, pos_y, angle);
  beams.push_back(beam);
  return beam;
}

WormBody* World::create_worm(float pos_x, float pos_y) {
  WormBody* worm = new WormBody(world, pos_x, pos_y);
  worms.push_back(worm);
  return worm;
}

void World::step(float timeStep, int32 velocityIterations, 
				 int32 positionIterations) {
  world->Step(timeStep, velocityIterations, positionIterations);
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

  delete world;
}


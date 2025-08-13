#include "world.h"
#include "box2d/box2d.h"
#include <stdio.h>

World::World() : world(std::make_shared<b2World>(b2Vec2(0.0f, -10.0f)))  {
  world->SetContactListener(&contact_listener);
}

BeamBody* World::create_beam(float pos_x, float pos_y, int angle, float length) {
  BeamBody* beam = new BeamBody(world.get(), pos_x, pos_y, angle, length);
  beams.push_back(beam);
  //beam->print_beam();
  return beam;
}

WormBody* World::create_worm(float pos_x, float pos_y, float vel, int health, const uint8_t& player_id) {
  WormBody* worm = new WormBody(world.get(), pos_x, pos_y, vel, health, player_id);
  worms.push_back(worm);
  return worm;
}

void World::create_water(float pos_x, float pos_y, float width, int height) {
  WaterBody* water = new WaterBody(world.get(), pos_x, pos_y, width, height);
  bodies.push_back(water);
}

void World::create_bazooka_missile(float pos_x, float pos_y) {
	Bazooka* bazooka = new Bazooka(world.get(), pos_x, pos_y);
	bodies.push_back(bazooka);
}

void World::create_banana(float pos_x, float pos_y) {
	Banana* banana = new Banana(world.get(), pos_x, pos_y);
	bodies.push_back(banana);
}

void World::create_dynamite(float pos_x, float pos_y) {
	Dynamite* dynamite = new Dynamite(world.get(), pos_x, pos_y);
	bodies.push_back(dynamite);
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
  for (auto &current_missile : missiles) {
    delete current_missile;
  }
}


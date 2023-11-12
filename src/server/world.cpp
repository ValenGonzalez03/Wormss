#include "box2d/box2d.h"
#include "world.h"
#include <stdio.h>

World::World() {
	b2Vec2 gravity(0.0f, -10.0f);
	world = new b2World(gravity);
}

Beam* World::create_beam(float pos_x, float pos_y) {
	Beam* beam = new Beam(world, pos_x, pos_y);
	beams.push_back(beam);
	return beam;
}

Worm* World::create_worm(float pos_x, float pos_y) {
	Worm* worm = new Worm(world, pos_x, pos_y);
	worms.push_back(worm);
	return worm;
}

void World::step(float timeStep, int32 velocityIterations, int32 positionIterations) {
	world->Step(timeStep, velocityIterations, positionIterations);
}

void World::delete_worms() {
	for (auto& worm: worms) {
        delete worm;
    }
    worms.clear();
}

void World::delete_beams() {
	for (auto& beam: beams) {
        delete beam;
    }
    beams.clear();
}

World::~World()
{
	for (auto& current_worm: worms) {
        delete current_worm;
    }
    
    for (auto& current_beam: beams) {
        delete current_beam;
    }
    
	delete world;
}

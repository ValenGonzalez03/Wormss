#ifndef WORLD_H
#define WORLD_H

#include "box2d/box2d.h"
#include <stdio.h>
#include <list>
#include "worm.h"
#include "beam.h"

class World {
	private:
	b2World* world;
	std::list<Worm*> worms;
	std::list<Beam*> beams;

	public:
	explicit World();
	
	Beam* create_beam(float pos_x, float pos_y);
	
	Worm* create_worm(float pos_x, float pos_y);
	
	void step(float timeStep, int32 velocityIterations, int32 positionIterations);
	
	void delete_worms();
	
	void delete_beams();
	
	~World();
	
	World(const World&) = delete;
	World& operator=(const World&) = delete;
};

#endif

#ifndef WORLD_H
#define WORLD_H

#include "box2d/box2d.h"
#include <stdio.h>
#include <list>
#include "worm_body.h"
#include "beam_body.h"

class World {
	private:
	b2World* world;
	std::list<WormBody*> worms;
	std::list<BeamBody*> beams;

	public:
	explicit World();
	
	BeamBody* create_beam(float pos_x, float pos_y);
	
	WormBody* create_worm(float pos_x, float pos_y);
	
	void step(float timeStep, int32 velocityIterations, int32 positionIterations);
	
	void delete_worms();
	
	void delete_beams();
	
	~World();
	
	World(const World&) = delete;
	World& operator=(const World&) = delete;
};

#endif

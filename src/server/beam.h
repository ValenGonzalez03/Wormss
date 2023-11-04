#ifndef BEAM_H
#define BEAM_H

#include "box2d/box2d.h"
#include <stdio.h>

class Beam {
	private:
	b2World* world;
	b2Body* body;
	const float pos_x; 
	const float pos_y;
	float angle = 0;
	const float width = 6;
	const float height = 0.80;
	float density = 1;
	float friction = 0;

	public:
	explicit Beam(b2World* world, float pos_x, float pos_y);

};

#endif

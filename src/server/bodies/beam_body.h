#ifndef BEAM_BODY_H
#define BEAM_BODY_H

#include "box2d/box2d.h"
#include "body.h"
#include <stdio.h>
#include <iostream>

class WormBody;

class BeamBody : public Body {
	private:
	b2World* world;
	b2Body* body;
	const float pos_x; 
	const float pos_y;
	int angle = 0;
	const float width = 6;
	const float height = 0.8;
	float density = 1;
	float friction = 0.5;

	public:
	explicit BeamBody(b2World* world, float pos_x, float pos_y);
	
	void start_contact_with(Body* another_body) override;
	
	void start_contact_with(WormBody* worm) override;
	
    void end_contact_with(Body* another_body) override;
	
	BeamBody(const BeamBody&) = delete;
	BeamBody& operator=(const BeamBody&) = delete;

	explicit BeamBody(b2World* world, float pos_x, float pos_y, int angle, float length);

	void print_beam() {
		std::cout << "BEAM" << std::endl;
		std::cout << "pos_x: " << pos_x << std::endl;
		std::cout << "pos_y: " << pos_y << std::endl;
		std::cout << "width: " << width << std::endl;
		std::cout << "height: " << height << std::endl;
	}

	float get_pos_x() {
		return this->pos_x;
	}

	float get_pos_y() {
		return this->pos_y;
	}

	int get_angle() {
		return this->angle;
	}

	float get_width() {
		return this->width;
	}
};

#endif

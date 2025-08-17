#ifndef BANANA_H
#define BANANA_H

#include "weapon.h"
#include "../../bodies/body.h"

class Banana : public Weapon {
	public:
    explicit Banana(b2World* world, float pos_x, float pos_y);

	// void start_contact_with(Body* another_body) override;
	
	// void end_contact_with(Body* another_body) override;
	
	int get_type() override;
};

#endif

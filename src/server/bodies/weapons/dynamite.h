#ifndef DYNAMITE_H
#define DYNAMITE_H

#include "weapon.h"
#include "../../bodies/body.h"

class Dynamite : public Weapon {
	public:
    explicit Dynamite(b2World* world, float pos_x, float pos_y);
    
    //void start_contact_with(Body* another_body) override;
	
	//void end_contact_with(Body* another_body) override;
	
	int get_type() override;
};

#endif

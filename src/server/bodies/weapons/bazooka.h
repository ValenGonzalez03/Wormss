#ifndef BAZOOKA_H
#define BAZOOKA_H

#include "weapon.h"
#include "../../../server/explosion_manager.h"
#include "../../bodies/body.h"
#include <iostream>

class Bazooka : public Weapon {
	private:
	ExplosionManager explosion_manager;
	
	public:
    explicit Bazooka(b2World* world, float pos_x, float pos_y);
    
    void shoot();
    
    void explode();
    
    // void start_contact_with(Body* another_body) override;
	
	// void end_contact_with(Body* another_body) override;
	
	int get_type() override;
};
#endif

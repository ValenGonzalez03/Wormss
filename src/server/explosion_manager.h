#ifndef EXPLOSION_MANAGER_H
#define EXPLOSION_MANAGER_H

#include "box2d/box2d.h"
#include "query_callback.h"
#include "bodies/body.h"

class ExplosionManager {
private:
  b2World* world;

public:  
	ExplosionManager(b2World *world);
	
	void simulate_explosion(b2Vec2 center, float blastRadius, float blastPower);

	void applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);
};

#endif 

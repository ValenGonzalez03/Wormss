//Fuente: https://www.iforce2d.net/b2dtut/explosions

#include "explosion_manager.h"

ExplosionManager::ExplosionManager(b2World *world): world(world) {}

void ExplosionManager::simulate_explosion(b2Vec2 center, float blastRadius, float blastPower){
	QueryCallback queryCallback;
	b2AABB aabb;
	aabb.lowerBound = center - b2Vec2(blastRadius, blastRadius);
	aabb.upperBound = center + b2Vec2(blastRadius, blastRadius);
	world->QueryAABB(&queryCallback, aabb);
  
	for (int i = 0; i < queryCallback.foundBodies.size(); i++) {
		b2Body* body = queryCallback.foundBodies[i];
		b2Vec2 bodyCom = body->GetWorldCenter();
      
		if ((bodyCom - center).Length() >= blastRadius)
			continue;
          
		applyBlastImpulse(body, center, bodyCom, blastPower);
	}
}


void ExplosionManager::applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower) {
	b2Vec2 blastDir = applyPoint - blastCenter;
    float distance = blastDir.Normalize();
      
    float invDistance = 1 / distance;
    float impulseMag = blastPower * invDistance * invDistance;
	body->ApplyLinearImpulse(impulseMag * blastDir, applyPoint, true);
}

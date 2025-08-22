//Fuente: https://www.iforce2d.net/b2dtut/explosions

#include "server_explosion.h"

Explosion::Explosion(float pos_x, float pos_y, float radius) : pos_x(pos_x), pos_y(pos_y), radius(radius) 
{}

void Explosion::update() {
	time_since_explosion_started += 1.0f;
}

bool Explosion::has_ended() {
	return time_since_explosion_started >= 3 * 60.0f;
}

void Explosion::update_ray_fraction(int ray_number, float new_fraction) {
	fraction_rays.assign(ray_number, new_fraction);
}

float Explosion::get_pos_x() {
	return pos_x;
}

float Explosion::get_pos_y() {
	return pos_y;
}

float Explosion::get_radius() {
	return radius;
}

std::vector<float> Explosion::get_fraction_rays() {
	return fraction_rays;
}

// void Explosion::simulate_explosion(b2Vec2 center, float blastRadius, float blastPower){
// 	QueryCallback queryCallback;
// 	b2AABB aabb;
// 	aabb.lowerBound = center - b2Vec2(blastRadius, blastRadius);
// 	aabb.upperBound = center + b2Vec2(blastRadius, blastRadius);
// 	world->QueryAABB(&queryCallback, aabb);
  
// 	for (int i = 0; i < queryCallback.foundBodies.size(); i++) {
// 		b2Body* body = queryCallback.foundBodies[i];
// 		b2Vec2 bodyCom = body->GetWorldCenter();
      
// 		if ((bodyCom - center).Length() >= blastRadius)
// 			continue;
          
// 		applyBlastImpulse(body, center, bodyCom, blastPower);
// 	}
// }


// void Explosion::applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower) {
// 	b2Vec2 blastDir = applyPoint - blastCenter;
//     float distance = blastDir.Normalize();
      
//     float invDistance = 1 / distance;
//     float impulseMag = blastPower * invDistance * invDistance;
// 	body->ApplyLinearImpulse(impulseMag * blastDir, applyPoint, true);
// }

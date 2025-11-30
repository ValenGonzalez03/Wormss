//Fuente: https://www.iforce2d.net/b2dtut/explosions

#include "server_explosion.h"
#include <iostream>

#define BLAST_POWER 15.0f

Explosion::Explosion(float pos_x, float pos_y, float radius) : pos_x(pos_x), pos_y(pos_y), radius(radius) 
{}

void Explosion::update() {
	time_since_explosion_started += 1.0f;
}

bool Explosion::has_ended() {
	return time_since_explosion_started >= 3 * 60.0f;
}

void Explosion::update_ray_fraction(int ray_number, float new_fraction) {
	fraction_rays[ray_number] = new_fraction;
}

void Explosion::try_add_affected_body(Body *body) {
	if ((affected_bodies.find(body) == affected_bodies.end()) && (body->is_affected_by_explosions())) {
		affected_bodies[body] = BodyExplosionInfo {b2Vec2(0,0), b2Vec2(0,0), 0};
	}
}

void Explosion::apply_explosion_to_bodies() {
	for (auto& [body_ptr, info] : affected_bodies) {
		BodyExplosionInfo expl_info = body_ptr->get_explosion_info();
		apply_explosion_impulse(body_ptr, expl_info, BLAST_POWER);
	}
}

void Explosion::apply_explosion_impulse(Body* body, BodyExplosionInfo explosion_info, float blast_power) {
	b2Body* body_b2 = body->get_body();
	float explotion_intensity = 1 - explosion_info.fraction_force;
	float impulse_mag = blast_power * explotion_intensity;
	std::cout << "fraction: " << explosion_info.fraction_force << std::endl;
	std::cout << "explotion_intensity: " << explotion_intensity << std::endl;
	body_b2->ApplyLinearImpulse(impulse_mag * explosion_info.impulse_dir, explosion_info.apply_point, true);
}

b2Vec2 Explosion::get_center() {
	return b2Vec2(pos_x, pos_y);
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



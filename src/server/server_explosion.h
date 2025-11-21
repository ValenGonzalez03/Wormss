#ifndef SERVER_EXPLOSION_H
#define SERVER_EXPLOSION_H

#include "box2d/box2d.h"
#include "../common/game_constants.h"
#include "query_callback.h"
#include "bodies/body.h"
#include <map>

struct BodyRayInfo {
	Body* body;
	b2Vec2 point;
	b2Vec2 normal;
	float fraction;
};

class Explosion {
private:
	float pos_x;
	float pos_y;
	float radius;
	float time_since_explosion_started = 0;
	std::vector<float> fraction_rays = std::vector<float>(NUM_RAYS, 1);
	std::map<Body*,BodyExplosionInfo> affected_bodies;

	void apply_explosion_impulse(Body* body, BodyExplosionInfo explosion_info, float blast_power);
public:  
	Explosion(float pos_x, float pos_y, float radius);

	void update();

	bool has_ended();
	
	void update_ray_fraction(int ray_number, float new_fraction);

	void try_add_affected_body(Body *body);

	void apply_explosion_to_bodies();

	float get_pos_x();

	float get_pos_y();

	float get_radius();

	std::vector<float> get_fraction_rays();

	//void simulate_explosion(b2Vec2 center, float blastRadius, float blastPower);

	//void applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);
};

#endif

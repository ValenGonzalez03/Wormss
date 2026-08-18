// Fuente: https://www.iforce2d.net/b2dtut/explosions

#include "server_explosion.h"
#include <iostream>

Explosion::Explosion(float pos_x, float pos_y, float radius, float charge_intensity) :
    pos_x(pos_x), pos_y(pos_y), radius(radius), charge_intensity(charge_intensity) {}

void Explosion::update() { time_since_explosion_started += 1.0f; }

bool Explosion::has_ended() { return time_since_explosion_started >= 3 * 60.0f; }

void Explosion::update_ray_fraction(int ray_number, float new_fraction) {
  fraction_rays[ray_number] = new_fraction;
}

void Explosion::try_add_affected_body(Body* body) {
  if ((affected_bodies.find(body) == affected_bodies.end()) &&
      (body->is_affected_by_explosions())) {
    affected_bodies[body] = BodyExplosionInfo{b2Vec2(0, 0), b2Vec2(0, 0), 0};
  }
}

void Explosion::apply_explosion_to_bodies() {
  for (auto& [body_ptr, info] : affected_bodies) {
    BodyExplosionInfo expl_info = body_ptr->get_explosion_info();
    apply_explosion_impulse(body_ptr, expl_info, charge_intensity);
  }
}

void Explosion::apply_explosion_impulse(Body* body, BodyExplosionInfo explosion_info,
                                        float charge_intensity) {
  float explotion_intensity = 1 - explosion_info.fraction_force;
  float impulse_mag = charge_intensity * explotion_intensity * BLAST_POWER;
  std::cout << "fraction: " << explosion_info.fraction_force << std::endl;
  std::cout << "explotion_intensity: " << explotion_intensity << std::endl;
  body->apply_impulse(impulse_mag * explosion_info.impulse_dir,
                      explosion_info.apply_point);
}

b2Vec2 Explosion::get_center() { return b2Vec2(pos_x, pos_y); }

float Explosion::get_radius() { return radius; }

std::vector<float> Explosion::get_fraction_rays() { return fraction_rays; }

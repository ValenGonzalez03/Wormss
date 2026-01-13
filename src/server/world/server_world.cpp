#include "server_world.h"
#include "box2d/box2d.h"
#include "explosion_callback.h"
#include <stdio.h>
#include <chrono>

#define EXPLOSION_RADIUS 3

World::World() : world(std::make_shared<b2World>(b2Vec2(0.0f, -10.0f)))  {
  world->SetContactListener(&contact_listener);
}

BeamBody* World::create_beam(float pos_x, float pos_y, int angle, float length) {
  float angle_radians = static_cast<float>(angle) * b2_pi / 180.0f;
  BeamBody* beam = new BeamBody(world.get(), pos_x, pos_y, angle_radians, length);
  beams.push_back(beam);
  //std::cout << "Creating beam of angle: " << beam->get_angle() << std::endl;
  beam->print_beam();
  return beam;
}

WormBody* World::create_worm(const uint8_t player_id, float spawn_x, float spawn_y, GameConfig& config) {
  WormBody* worm = new WormBody(world.get(), spawn_x, spawn_y, config.get_worm_speed(), config.get_worm_life(), player_id);
  worms.push_back(worm);
  return worm;
}

MissileBody* World::create_missile(uint8_t id, float pos_x, float pos_y, float angle, uint8_t direction, float initial_force) {
  MissileBody* missile = new MissileBody(world.get(), pos_x, pos_y, angle, direction, id);
  missile->apply_initial_impulse(initial_force, angle);
  explodables.push_back(missile);
  //std::cout << "Angle: " << angle << "  Direction: " << (int)direction << "  Initial force: " << initial_force << std::endl;
  return missile;
}

GrenadeBody* World::create_grenade(uint8_t id, float pos_x, float pos_y, float angle, uint8_t direction, float initial_force) {
  GrenadeBody* grenade = new GrenadeBody(world.get(), pos_x, pos_y, angle, direction, id);
  grenade->apply_initial_impulse(initial_force, angle);
  explodables.push_back(grenade);
  return grenade;
}

void World::create_explosion(float pos_x, float pos_y) {
  Explosion explosion (pos_x, pos_y, EXPLOSION_RADIUS);
  for (int i = 0; i < NUM_RAYS; i++) {
    float angle_rad = (i / (float)NUM_RAYS) * 2.0f * b2_pi;
    b2Vec2 center (pos_x, pos_y);
    b2Vec2 ray_dir ( EXPLOSION_RADIUS * sinf(angle_rad), EXPLOSION_RADIUS * cosf(angle_rad) );
    b2Vec2 ray_end = center + ray_dir;

    ExplosionCallback callback (i, explosion);
    world->RayCast(&callback, center, ray_end);

    //std::cout << "Ray fraction n° " << i << ": " << callback.get_ray_fraction() << std::endl;
    explosion.update_ray_fraction(i, callback.get_ray_fraction());
    callback.evaluate_contact_for_bodies();
  }
  explosion.apply_explosion_to_bodies();
  explosions.push_back(explosion);
}

void World::destroy_body(Body* body) {
  world->DestroyBody(body->body);
  delete body;
}

void World::step(float timeStep, int32 velocityIterations, int32 positionIterations) {
  world->Step(timeStep, velocityIterations, positionIterations);
}

WormBody* World::get_worm(const uint8_t &player_id) {
  for (auto it = worms.begin(); it != worms.end(); ++it) {
    if ((*it)->get_id() == player_id) {
      return (*it);
    }
  }
  return nullptr;
}

void World::update_worms() {
  for (WormBody *worm : worms) {
    worm->update();
  }
}

void World::update_explodables() {
  for (std::list<Explodable*>::iterator it = explodables.begin(); it != explodables.end();)
    {
      if ((*it)->has_exploded()) {
        create_explosion((*it)->get_pos_x(), (*it)->get_pos_y());
        destroy_body(*it);
        it = explodables.erase(it);
        std::cout << "KABOOM" << std::endl;
      } else {
        (*it)->update();
        it++;
      }
    }
}

void World::update_explosions() {
  for (std::list<Explosion>::iterator it = explosions.begin(); it != explosions.end();)
    {
      if ((it)->has_ended()) {
        it = explosions.erase(it);
      } else {
        (it)->update();
        it++;
      }
    }
}

int World::get_worms_number() {
  return worms.size();
}

int World::get_explodables_number() {
  return explodables.size();
}

void World::ray_cast(b2RayCastCallback *callback, const b2Vec2 &point1, const b2Vec2 &point2) {
  world->RayCast(callback, point1, point2);
}

std::list<WormAttr> World::get_worms_attr() {
  std::list<WormAttr> worms_attr;
  for (auto worm : worms) {
    WormAttr attr ({worm->get_id(), worm->get_pos_x(), worm->get_pos_y(), worm->get_direction(), worm->get_state(), 
                    worm->get_weapon_selected(), worm->get_aiming_angle()});
    worms_attr.emplace_back(attr);
  }
  return worms_attr;
}

std::list<ExplodableAttr> World::get_explodables_attr() {
  std::list<ExplodableAttr> expl_attr;
  for (auto explodable : explodables) {
    ExplodableAttr attr {explodable->get_id(), explodable->get_type(), explodable->get_pos_x(), explodable->get_pos_y(), 
      explodable->get_angle(), explodable->get_direction()};
      expl_attr.emplace_back(attr);
    std::cout << "Getting angle from body: " << explodable->get_angle() << std::endl;
  }
  return expl_attr;
}

std::list<ExplosionAttr> World::get_explosions_attr() {
  std::list<ExplosionAttr> explosions_attr;
  for (auto explosion : explosions) {
    b2Vec2 center = explosion.get_center();
    ExplosionAttr attr ({center.x, center.y, explosion.get_radius(), explosion.get_fraction_rays()});
    explosions_attr.emplace_back(attr);
  }
  return explosions_attr;
}

void World::delete_worms() {
  for (auto &worm : worms) {
	  destroy_body(worm);
  }
  worms.clear();
}

void World::delete_beams() {
  for (auto &beam : beams) {
    destroy_body(beam);
  }
  beams.clear();
}

void World::delete_explodables() {
  for (auto &explodable : explodables) {
    destroy_body(explodable);
  }
  explodables.clear();
}

World::~World() {
  delete_worms();

  delete_beams();

  delete_explodables();
}


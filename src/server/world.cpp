#include "world.h"
#include "box2d/box2d.h"
#include "explosion_callback.h"
#include <stdio.h>
#include <chrono>

#define EXPLOSION_RADIUS 3

World::World() : world(std::make_shared<b2World>(b2Vec2(0.0f, -10.0f)))  {
  world->SetContactListener(&contact_listener);
}

BeamBody* World::create_beam(float pos_x, float pos_y, int angle, float length) {
  BeamBody* beam = new BeamBody(world.get(), pos_x, pos_y, angle, length);
  beams.push_back(beam);
  //beam->print_beam();
  return beam;
}

WormBody* World::create_worm(const uint8_t player_id, float spawn_x, float spawn_y, GameConfig& config) {
  WormBody* worm = new WormBody(world.get(), spawn_x, spawn_y, config.get_worm_speed(), config.get_worm_life(), player_id);
  worms.push_back(worm);
  return worm;
}

MissileBody* World::create_missile(MissileBody* missile) {
  missiles.push_back(missile);
  return missile;
}

void World::create_explosion(MissileBody* missile) {
  Explosion explosion (missile->get_pos_x(), missile->get_pos_y(), EXPLOSION_RADIUS);
  for (int i = 0; i < NUM_RAYS; i++) {
    float angle_rad = (i / (float)NUM_RAYS) * 2.0f * b2_pi;
    b2Vec2 center (missile->get_pos_x(), missile->get_pos_y());
    b2Vec2 ray_dir ( EXPLOSION_RADIUS * sin(angle_rad), EXPLOSION_RADIUS * cos(angle_rad) );
    b2Vec2 ray_end = center + ray_dir;

    ExplosionCallback callback (i, explosion);
    world->RayCast(&callback, center, ray_end);

    //std::cout << "Ray fraction n° " << i << ": " << callback.get_ray_fraction() << std::endl;
    explosion.update_ray_fraction(i, callback.get_ray_fraction());
    //callback.evaluate_contact_for_bodies();
  }
  explosions.push_back(explosion);
}

void World::destroy_body(Body* body) {
  world->DestroyBody(body->get_body());
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

void World::update_missiles() {
  for (std::list<MissileBody*>::iterator it = missiles.begin(); it != missiles.end();)
    {
      if ((*it)->has_exploded()) {
        create_explosion(*it);
        destroy_body(*it);
        it = missiles.erase(it);
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

int World::get_missiles_number() {
  return missiles.size();
}

std::list<WormAttr> World::get_worms_attr() {
  std::list<WormAttr> worms_attr;
  for (auto worm : worms) {
    WormAttr attr ({worm->get_id(), worm->get_pos_x(), worm->get_pos_y(), worm->get_direction(), worm->get_state(), worm->get_aiming_angle()});
    worms_attr.emplace_back(attr);
  }
  return worms_attr;
}

std::list<MissileAttr> World::get_missiles_attr() {
  std::list<MissileAttr> missiles_attr;
  for (auto missile : missiles) {
    MissileAttr attr ({missile->get_id(), missile->get_pos_x(), missile->get_pos_y(), missile->get_angle(), missile->get_direction()});
    missiles_attr.emplace_back(attr);
  }
  return missiles_attr;
}

std::list<ExplosionAttr> World::get_explosions_attr() {
  std::list<ExplosionAttr> explosions_attr;
  for (auto explosion : explosions) {
    ExplosionAttr attr ({explosion.get_pos_x(), explosion.get_pos_y(), explosion.get_radius(), explosion.get_fraction_rays()});
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

void World::delete_missiles() {
  for (auto &missile : missiles) {
    destroy_body(missile);
  }
  missiles.clear();
}

World::~World() {
  delete_worms();

  delete_beams();

  delete_missiles();
}


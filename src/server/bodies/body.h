#ifndef BODY_H
#define BODY_H

#include "box2d/box2d.h"
#include "../../common/game_constants.h"

class WormBody;
class BeamBody;
class MissileBody;
class GrenadeBody;

class World;

struct UserData {
  BODY_TYPES type;
  void* pointer;
};

struct BodyExplosionInfo {
  b2Vec2 apply_point;
  b2Vec2 impulse_dir;
  float fraction_force;
};

class Body {
 protected:
  b2Body* body;
  const uint8_t id;
  const float width;
  const float height;
  const float angle;
  bool affected_by_explosions = false;

  friend class World;

 public:
  Body(const BodyBasicData& basic_data, const BodyAdvData& adv_data, BODY_TYPES type,
       b2BodyType body_type, b2World* world);

  // Dado el tipo de another_body define el comportamiento del cuerpo al empezar un
  // contacto con el otro cuerpo
  void start_contact_with(Body* another_body, b2Fixture self_fixt);

  // Recibe el fixture que entró en contacto con el gusano y determina el comportamiento al
  // colisionar con el mismo
  virtual void start_contact_with_worm(b2Fixture self_fixt) {}
  // Recibe el fixture que entró en contacto con la viga y determina el comportamiento al
  // colisionar con el mismo
  virtual void start_contact_with_beam(b2Fixture self_fixt) {}
  // Recibe el fixture que entró en contacto con el misil y determina el comportamiento al
  // colisionar con el mismo
  virtual void start_contact_with_missile(b2Fixture self_fixt) {}
  // Recibe el fixture que entró en contacto con la granada y determina el comportamiento al
  // colisionar con el mismo
  virtual void start_contact_with_grenade(b2Fixture self_fixt) {}
  // Recibe el fixture que entró en contacto con el agua y determina el comportamiento al
  // colisionar con el mismo
  virtual void start_contact_with_water(b2Fixture self_fixt) {}

  // Dado el tipo de another_body define el comportamiento del cuerpo al terminar un
  // contacto con el otro cuerpo
  void end_contact_with(Body* another_body, b2Fixture self_fixt);

  // Recibe el fixture que dejo de entrar en contacto con el gusano y determina el
  // comportamiento al dejar de colisionar con el mismo
  virtual void end_contact_with_worm(b2Fixture self_fixt) {}
  // Recibe el fixture que dejo de entrar en contacto con la viga y determina el
  // comportamiento al dejar de colisionar con el mismo
  virtual void end_contact_with_beam(b2Fixture self_fixt) {}
  // Recibe el fixture que dejo de entrar en contacto con el misil y determina el
  // comportamiento al dejar de colisionar con el mismo
  virtual void end_contact_with_missile(b2Fixture self_fixt) {}
  // Recibe el fixture que dejo de entrar en contacto con la granada y determina el
  // comportamiento al dejar de colisionar con el mismo
  virtual void end_contact_with_grenade(b2Fixture self_fixt) {}
  // Recibe el fixture que dejo de entrar en contacto con el agua y determina el
  // comportamiento al dejar de colisionar con el mismo
  virtual void end_contact_with_water(b2Fixture self_fixt) {}

  virtual void take_damage(int amount);

  bool check_boundaries();

  void apply_impulse(const b2Vec2& impulse, const b2Vec2& point);

  virtual float explosion_intersect_value(float fraction) = 0;

  virtual void update_explosion_ray_contact(const b2Vec2& point, const b2Vec2& normal,
                                            float fraction) = 0;

  virtual BodyExplosionInfo get_explosion_info() = 0;

  // b2Body* get_body();

  uint8_t get_id();

  float get_pos_x();

  float get_pos_y();

  float get_width();

  float get_height();

  float get_angle();

  UserData* get_user_data();

  virtual BODY_TYPES get_type() = 0;

  bool is_affected_by_explosions();
  // virtual void start_contact_with(Weapon* weamon) = 0;

  Body(const Body&) = delete;
  Body& operator=(const Body&) = delete;
};

#endif

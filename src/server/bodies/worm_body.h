#ifndef WORM_BODY_H
#define WORM_BODY_H

#include "../../common/game_constants.h"
#include "dynamic_body.h"
#include "box2d/box2d.h"
#include <iostream>
#include <stdio.h>

class WormBody : public DynamicBody {
private:
  // Atributos fijos del gusano
  uint8_t id;
  float vel;
  int health;
  float jump_vel_forward = 5;
  float jump_vel_backward = 5;

  // Atributos de estado del gusano
  WormState state = IDLE;
  WeaponType current_weapon = BAZOOKA;
  uint8_t direction = RIGHT;
  uint8_t aim_direction;
  float aiming_angle = 0;
  int frames_attacking = 0;

  // Atributos para manejar la explosion sobre un gusano
  bool hit_by_explosion = false;
  b2Vec2 impulse_dir = b2Vec2(0,0);
  b2Vec2 apply_point = b2Vec2(0,0);
  int num_ray_contacts = 0;
  float fraction_force = 2.0f;

  bool m_contacting = false;
  int num_foot_contacts = 0;


public:
  //explicit WormBody(b2World* world, float pos_x, float pos_y, uint8_t id);

  // Constructor con pos_x, pos_y, velocidad, vida, etc
  explicit WormBody(b2World* world, float pos_x, float pos_y, float vel, int health, uint8_t id);
  
  void move_left();

  void move_right();

  void apply_horizontal_impulse(float desired_vel);

  void apply_vertical_impulse(float jump_speed);

  void start_moving(const uint8_t &dir);

  void stop_moving();

  void jump_backward();

  void jump_forward();

  void jump(const uint8_t &dir, const uint8_t &jump_type);

  void start_aiming(const uint8_t &dir);

  void aim_up();

  void aim_down();

  void stop_aiming();

  void change_weapon(WeaponType weapon);

  void attack();

  // Por ahora un shoot generico que solo va a ser usado para la bazooka
  ExplodableAttr attack_projectile(b2Vec2 proj_pos, uint8_t proj_id);

  b2Vec2 calculate_projectile_launch_position(float proj_width, float proj_height, float offset_x = 0.0f, float offset_y = 0.0f);
  
  void teleport(float pos_x, float pos_y);

  float explosion_intersect_value(float fraction) override;

  void update_explosion_ray_contact(b2Vec2& point, b2Vec2& center_expl, float fraction) override;

  BodyExplosionInfo get_explosion_info() override;

  uint8_t get_id();

  b2Vec2 get_position();

  float get_pos_x();

  float get_pos_y();

  uint8_t get_direction();

  WormState get_state();

  float get_aiming_angle();

  WeaponType get_weapon_selected();

  void set_worm_to_attack();

  void update();

  bool is_facing_left();

  bool is_facing_right();

  bool is_stopped();
  
  bool is_inactive();

  void touch_worm(WormBody* worm) override;
  void touch_beam(BeamBody* beam) override;
  void touch_missile(MissileBody* missile) override;
  void touch_grenade(GrenadeBody* grenade) override;

  void stop_touching_worm(WormBody* worm) override;
  void stop_touching_beam(BeamBody* beam) override;
  void stop_touching_missile(MissileBody* missile) override;
  void stop_touching_grenade(GrenadeBody* grenade) override;

  // POR AHORA DE PRUEBA
  void start_contact();
  void end_contact();

  //void start_contact_with(Body *another_body) override;

  //void end_contact_with(Body *another_body) override;

  void hit_a_surface();

  void move_away_from_surface();

  void show_vel_and_health();
  
  void take_damage(int damage);
  
  void die();
  
  void shoot_bazooka();
  
  BODY_TYPES get_type() override;

  ~WormBody();
  
  WormBody(const WormBody&) = delete;
  WormBody& operator=(const WormBody&) = delete;
};

#endif

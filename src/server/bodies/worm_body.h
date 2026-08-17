#ifndef WORM_BODY_H
#define WORM_BODY_H

#include "../../common/game_constants.h"
#include "dynamic_body.h"
#include "box2d/box2d.h"
#include "../game/weapons/server_weapon.h"
#include <memory>
#include <iostream>
#include <stdio.h>

class WormBody : public DynamicBody {
 private:
  // Atributos fijos del gusano
  int health;
  float vel;
  float jump_vel_forward = 5;
  float jump_vel_backward = 5;

  // WeaponType current_weapon = BAZOOKA;
  std::unique_ptr<ServerWeapon> current_weapon;
  // Atributos de estado del gusano
  WormState state = IDLE;
  uint8_t direction = RIGHT;

  uint8_t aim_direction = UP;
  float aiming_angle = 0;

  int num_foot_contacts = 0;
  int jump_timeout = 0;

  int frames_attacking = 0;


  // Atributos para manejar la explosion sobre un gusano
  bool hit_by_explosion = false;
  b2Vec2 impulse_dir = b2Vec2(0, 0);
  b2Vec2 apply_point = b2Vec2(0, 0);
  int num_ray_contacts = 0;
  float fraction_force = 2.0f;

  void apply_horizontal_impulse(float desired_vel);

  void apply_vertical_impulse(float jump_speed);

 public:
  // explicit WormBody(b2World* world, float pos_x, float pos_y, uint8_t id);

  explicit WormBody(const BodyBasicData& basic_data, const BodyAdvData& adv_data,
                    int health, float vel, b2World* world);

  void update();



  //////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////// METODOS DE CONTROL DEL GUSANO /////////////////////////////////

  void start_moving(const uint8_t& dir);
  void move_left();
  void move_right();
  void stop_moving();


  void jump(const uint8_t& dir, const uint8_t& jump_type);
  void jump_forward();
  void jump_backward();


  void start_aiming(const uint8_t& dir);
  void aim_up();
  void aim_down();
  void stop_aiming();


  void change_weapon(WeaponType weapon);

  ///////////////////////////////// METODOS DE CONTROL DE GUSANO /////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////



  ///////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////// METODOS DE ARMAS Y EXPLOSIONES /////////////////////////////////

  // Calcula la posicion de lanzamiento de un proyectil en base a la del gusano, aplicando corrimientos opcionales
  b2Vec2 calculate_projectile_launch_position(float proj_width, float proj_height,
                                              float offset_x = 0.0f,
                                              float offset_y = 0.0f);

  // Devuelve los atributos del proyectil a lanzar
  ExplodableAttr attack_projectile(b2Vec2 proj_pos, uint8_t proj_id);

  void set_to_charge();

  void set_to_attack();

  float explosion_intersect_value(float fraction) override;

  void update_explosion_ray_contact(const b2Vec2& point, const b2Vec2& center_expl,
                                    float fraction) override;

  BodyExplosionInfo get_explosion_info() override;

  void teleport(float pos_x, float pos_y);

  ///////////////////////////////// METODOS DE ARMAS Y EXPLOSIONES /////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////



  /////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////// GETTERS ///////////////////////////////////////////

  BODY_TYPES get_type() override;

  WormState get_state();

  ServerWeapon* get_weapon_selected();

  uint8_t get_direction();

  float get_aiming_angle();

  ///////////////////////////////////////// GETTERS ///////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////



  /////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////// COLISIONES ///////////////////////////////////////////

  void touch_worm(WormBody* worm) override;
  void touch_beam(BeamBody* beam) override;
  void touch_missile(MissileBody* missile) override;
  void touch_grenade(GrenadeBody* grenade) override;

  void stop_touching_worm(WormBody* worm) override;
  void stop_touching_beam(BeamBody* beam) override;
  void stop_touching_missile(MissileBody* missile) override;
  void stop_touching_grenade(GrenadeBody* grenade) override;

  void hit_a_surface();

  void move_away_from_surface();

  ///////////////////////////////////////// COLISIONES ///////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////////////////


  ~WormBody();

  WormBody(const WormBody&) = delete;
  WormBody& operator=(const WormBody&) = delete;
};

#endif

#ifndef GAME_CONSTANTS_H_
#define GAME_CONSTANTS_H_

#include <cstdint>
#include <vector>
#include "box2d/box2d.h"

#define PIXEL_PER_METER 25.0f  // 23,3 periodico (1 metro equivale a 25 pixeles)
#define WORLD_WIDTH 50.0f      // En metros (Equivale a 1250 pixeles)
#define WORLD_HEIGHT 30.0f     // En metros (Equivale a 750 pixeles)

#define WORM_WIDTH 0.8f   // En metros (Equivale a 20 pixeles)
#define WORM_HEIGHT 1.0f  // En metros (Equivale a 25 pixeles)

#define BEAM_HEIGHT 0.85f  // En metros (Equivale a 21,25 pixeles)

#define MISSILE_WIDTH 1.0f   // En metros (Equivale a 25 pixeles)
#define MISSILE_HEIGHT 0.8f  // En metros (Equivale a 20 pixeles)

#define GRENADE_WIDTH 0.6f   // En metros (Equivale a 15 pixeles)
#define GRENADE_HEIGHT 0.8f  // En metros (Equivale a 20 pixeles)

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#define JUMP_FORWARD 0
#define JUMP_BACKWARD 1

enum WormState { IDLE, MOVING, JUMPING, AIMING, ATTACKING };

enum WeaponType { NONE, BAZOOKA, BAT, GRENADE };

struct texture_vals {
  int x;
  int y;
  int width;
  int height;

  texture_vals operator+(const texture_vals &other) {
    return {x + other.x, y + other.y, width + other.width, height + other.height};
  }
};

enum BODY_TYPES { WORM = 0, WATER = 1, BEAM = 2, MISSILE = 3, GRENADE_BODY = 4 };

enum BODY_CATEGORIES {
  WORM_CATEGORY = 0x0001,
  BEAM_CATEGORY = 0x0002,
  MISSILE_CATEGORY = 0x0003,
  GRENADE_CATEGORY = 0x0004
};

struct BodyBasicData {
  uint8_t id;
  float pos_x;
  float pos_y;
  float angle;
  float width;
  float height;
};

struct BodyAdvData {
  float density;
  float friction;
  uint16 category_bits;
  uint16 mask_bits;
};

struct WormAttr {
  uint8_t player_id;
  float pos_x;  // En metros
  float pos_y;  // En metros
  uint8_t direction;
  WormState state;
  WeaponType current_weapon;
  float aim_angle;  // En radianes
};

struct BeamAttr {
  float pos_x;  // En metros
  float pos_y;  // En metros
  float angle;  // En radianes
  float width;  // En metros
};

struct ExplodableAttr {
  uint8_t id;
  BODY_TYPES type;
  float pos_x;  // En metros
  float pos_y;  // En metros
  float angle;  // En radianes
  uint8_t direction;
};

#define NUM_RAYS 16

struct ExplosionAttr {
  float pos_x;
  float pos_y;
  float radius;
  std::vector<float> ray_fractions;
};

#endif
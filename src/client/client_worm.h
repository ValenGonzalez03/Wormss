#ifndef CLIENT_WORM_H_
#define CLIENT_WORM_H_

#include "../common/game_state.h"
#include "../common/game_constants.h"
#include "client_resource_pool.h"


#define FIRST_BAZ_TEXT 0
#define FIRST_BAT_TEXT 1

class WeaponTextureList {
private:
  std::vector<std::vector<SDL2pp::Texture *>> weapon_textures;

public:
  WeaponTextureList(ResourcePool &res_pool) {
    weapon_textures.push_back(res_pool.get_worm_aiming(BAZOOKA));
    weapon_textures.push_back(res_pool.get_worm_aiming(BAT));
  }

  std::vector<SDL2pp::Texture*> get_aim_texture(WeaponType type) {
    switch (type) {
      case BAZOOKA:
        return weapon_textures[FIRST_BAZ_TEXT];
      case BAT:
        return weapon_textures[FIRST_BAT_TEXT];
      default:
        throw std::runtime_error("Weapon type not supported");
    }
  }

  std::vector<SDL2pp::Texture*> get_attack_texture(WeaponType type) {
    switch (type) {
      case BAZOOKA:
        return weapon_textures[FIRST_BAZ_TEXT];
      case BAT:
        return weapon_textures[FIRST_BAT_TEXT + 1];
      default:
        throw std::runtime_error("Weapon type not supported");
    }
  }
};

class Worm {
private:
  int id;
  int pos_x; // En pixeles
  int pos_y; // En pixeles
  int width; // En pixeles
  int height; // En pixeles
  float aim_angle;
  uint8_t direction;
  WormState worm_state;
  WeaponType weapon;
  std::vector<std::vector<SDL2pp::Texture *>> textures; // Vector de grillas de texturas
  WeaponTextureList weapon_textures;
  SDL2pp::Renderer &renderer;
  // SDL2pp::Texture &shooting_texture;

  SDL_RendererFlip choose_flip_direction();

public:

  explicit Worm();
  // Crea un Worm con un renderer y las texturas correspondientes
  explicit Worm(int id, int pos_x, int pos_y, int width, int heigth, float aim_angle, uint8_t direction, 
    WormState worm_state, std::vector<std::vector<SDL2pp::Texture *>> &&textures, SDL2pp::Renderer &rend, ResourcePool &res_pool);

  int get_id();

  void update(WormData data);

  // Renderiza el worm pasado por parámetro según el estado del
  // mismo
  void render(int frame);

  // Renderiza el worm cuando este se encuentra en estado "idle"
  void render_worm_idle(int frame);

  // Renderiza el worm cuando este se encuentra en estado "running"
  void render_worm_running(int frame);

  // Renderiza al worm saltando
  void render_worm_jumping(int frame);

  // Renderiza el worm apuntando cuando este se encuentra en estado "aiming"
  void render_worm_aiming(int frame);
};


#endif
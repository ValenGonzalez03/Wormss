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
    weapon_textures.push_back(res_pool.get_worm_attacking(BAT));
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
  WeaponTextureList weapon_textures; // Objeto que contiene las texturas de todas las armas
  SDL2pp::Renderer &renderer;
  // SDL2pp::Texture &shooting_texture;

  SDL_RendererFlip choose_flip_direction();

  // Renderiza el worm cuando se encuentra en estado "idle"
  void render_worm_idle(int frame);

  // Renderiza el worm cuando se encuentra en estado "running"
  void render_worm_running(int frame);

  // Renderiza al worm cuando se encuentra en estado "jumping" (NO TERMINADA)
  void render_worm_jumping(int frame);

  // Renderiza el worm cuando se encuentra en estado "aiming" en funcion del arma seleccionada
  void render_worm_aiming(int frame);

  // Renderiza el worm cuando se encuentra en estado "attacking" en funcion del arma seleccionada
  void render_worm_attacking(int frame);

public:

  explicit Worm();

  // Crea un Worm con un renderer y las texturas correspondientes
  explicit Worm(int id, int pos_x, int pos_y, int width, int heigth, float aim_angle, uint8_t direction, 
    WormState worm_state, std::vector<std::vector<SDL2pp::Texture *>> &&textures, SDL2pp::Renderer &rend, ResourcePool &res_pool);

  int get_id();

  // Actualiza el estado del Worm con la informacion del server
  void update(WormData data);

  // Renderiza el worm en función de su estado actual
  void render(int frame);

};


#endif
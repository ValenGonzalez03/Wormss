#ifndef CLIENT_WORM_H_
#define CLIENT_WORM_H_

#include "../../common/game_state.h"
#include "../../common/game_constants.h"
#include "../weapons/client_bazooka.h"
#include "../weapons/client_bat.h"
#include "../weapons/client_grenade.h"


class Worm {
 private:
  int id;
  int pos_x;   // En pixeles
  int pos_y;   // En pixeles
  int width;   // En pixeles
  int height;  // En pixeles
  float aim_angle;
  uint8_t direction;
  WormState worm_state;
  Weapon *weapon;
  ResourcePool &resource_pool;
  SDL2pp::Renderer &renderer;
  // SDL2pp::Texture &shooting_texture;

  void update_weapon_selected(WeaponType type);

  void assign_new_weapon(WeaponType type);

  SDL_RendererFlip choose_flip_direction();

  // Renderiza el worm cuando se encuentra en estado "idle"
  void render_worm_idle(int frame, int camera_x, int camera_y);

  // Renderiza el worm cuando se encuentra en estado "running"
  void render_worm_running(int frame, int camera_x, int camera_y);

  // Renderiza al worm cuando se encuentra en estado "jumping" (NO TERMINADA)
  void render_worm_jumping(int frame, int camera_x, int camera_y);

  // Renderiza el worm cuando se encuentra en estado "aiming" en funcion del arma seleccionada
  void render_worm_aiming(int frame, int camera_x, int camera_y);

  // Renderiza el worm cuando se encuentra en estado "attacking" en funcion del arma seleccionada
  void render_worm_attacking(int frame, int camera_x, int camera_y);

 public:
  Worm();

  // Crea un Worm con un renderer y las texturas correspondientes
  explicit Worm(int id, int pos_x, int pos_y, int width, int heigth, float aim_angle, uint8_t direction,
                WormState worm_state, SDL2pp::Renderer &rend, ResourcePool &res_pool);  // NOLINT(runtime/references)

  int get_id();

  // Actualiza el estado del Worm con la informacion del server
  void update(WormData data);

  // Renderiza el worm en función de su estado actual
  void render(int frame, int camera_x, int camera_y);

  int get_pos_x();
  int get_pos_y();
  int get_width();
  int get_height();

  ~Worm();
};


#endif

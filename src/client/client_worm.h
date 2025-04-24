#ifndef CLIENT_WORM_H_
#define CLIENT_WORM_H_

#include "../common/game_state.h"
#include "../common/worm_states.h"
#include "client_resource_pool.h"
#include "client_state.h"

class Worm {
private:
  int id;
  int pos_x; // En pixeles
  int pos_y; // En pixeles
  int width; // En pixeles
  int height; // En pixeles
  float aim_angle;
  uint8_t direction;
  client_state state; // Hay que ver que hago con esto
  uint8_t worm_state;
  std::vector<std::vector<SDL2pp::Texture *>> textures; // Vector de grillas de texturas
  SDL2pp::Renderer &renderer;
  // SDL2pp::Texture &shooting_texture;

  SDL_RendererFlip choose_flip_direction();

public:

  explicit Worm();
  // Crea un Worm con un renderer y las texturas correspondientes
  explicit Worm(int id, int pos_x, int pos_y, int width, int heigth, float aim_angle, uint8_t direction, 
    uint8_t state, std::vector<std::vector<SDL2pp::Texture *>> &&textures, SDL2pp::Renderer &rend);

  int get_id();

  void update(WormData data);

  // Renderiza el worm pasado por parámetro según el estado del
  // mismo
  void render(int frame, client_state &worm_state);

  // Renderiza el worm cuando este se encuentra en estado "idle"
  void render_worm_idle(int frame, client_state &worm_state);

  // Renderiza el worm cuando este se encuentra en estado "running"
  void render_worm_running(int frame, client_state &worm_state);

  // Renderiza al worm saltando
  void render_worm_jumping(int frame, client_state &worm_state);

  // Renderiza el worm apuntando cuando este se encuentra en estado "aiming"
  void render_worm_aiming(int frame);
};

#endif
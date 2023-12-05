#ifndef CLIENT_WORM_VIEW_H_
#define CLIENT_WORM_VIEW_H_

#include "../common/game_state.h"
#include "../common/worm_states.h"
#include "client_position_converter.h"
#include "client_resource_pool.h"
#include "client_state.h"

class WormView {
private:
  SDL2pp::Renderer &renderer;
  // SDL2pp::Texture &standing_texture;
  std::vector<SDL2pp::Texture *> &walking_texture;
  std::vector<SDL2pp::Texture *> &jumping_texture;
  std::vector<SDL2pp::Texture *> &aiming_texture;
  // SDL2pp::Texture &shooting_texture;

  SDL_RendererFlip choose_flip_direction(Worm &worm);

public:
  // Crea un WormView con un renderer y las texturas correspondientes
  explicit WormView(SDL2pp::Renderer &rend,
                    std::vector<SDL2pp::Texture *> &walking,
                    std::vector<SDL2pp::Texture *> &jumping,
                    std::vector<SDL2pp::Texture *> &aiming)
      : renderer(rend), walking_texture(walking), jumping_texture(jumping),
        aiming_texture(aiming) {}

  // Renderiza el worm pasado por parámetro según el estado del
  // mismo
  void render(int frame, Worm &worm, client_state &worm_state);

  // Renderiza el worm cuando este se encuentra en estado "idle"
  void render_worm_idle(int frame, int pos_x, int pos_y, Worm &worm,
                        client_state &worm_state);

  // Renderiza el worm cuando este se encuentra en estado "running"
  void render_worm_running(int frame, int pos_x, int pos_y, Worm &worm,
                           client_state &worm_state);

  // Renderiza al worm saltando
  void render_worm_jumping(int frame, int pos_x, int pos_y, Worm &worm,
                           client_state &worm_state);

  void render_worm_aiming(int frame, int pos_x, int pos_y, Worm &worm);
};

#endif
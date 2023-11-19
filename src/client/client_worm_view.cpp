#include "client_worm_view.h"

void WormView::render(int frame, Worm &worm, client_state &worm_state) {
  PositionConverter converter;
  int pos_x = converter.convert_from_m_to_px(worm.get_pos_x());
  int pos_y = converter.convert_from_m_to_px(worm.get_pos_y());
  if (worm_state.is_running)
      render_worm_running(pos_x, pos_y, worm, worm_state);
  else
      render_worm_idle(pos_x, pos_y, worm, worm_state);
}

void WormView::render_worm_idle(int pos_x, int pos_y, Worm &worm, client_state &worm_state) {
  walking_texture.SetAlphaMod(255);
  renderer.Copy(
    walking_texture, 
    SDL2pp::Rect(10, 10, 40, 40),          // Size
    SDL2pp::Rect(pos_x, pos_y, 40, 40),    // Destination
    0.0,                                   // don't rotate
    SDL2pp::NullOpt,                       // rotation center - not needed
    SDL_FLIP_HORIZONTAL                    // horizontal flip
);
}

void WormView::render_worm_running(int pos_x, int pos_y, Worm &worm, client_state &worm_state) {
  SDL_RendererFlip flip = SDL_FLIP_NONE; // Sin volteo por defecto

  int src_x = 10, src_y = 10; // by default, standing sprite
  // Voltear horizontalmente solo si te estás moviendo a la izquierda
  if (worm_state.direction == LEFT) {
      //flip = SDL_FLIP_NONE;
  }
  else if (worm_state.direction == RIGHT) {
    flip = SDL_FLIP_HORIZONTAL;
  }
  src_x = 10;
  src_y = 10 + 60 * worm_state.run_phase;

  walking_texture.SetAlphaMod(255); // sprite is fully opaque
  renderer.Copy(
    walking_texture, 
    SDL2pp::Rect(src_x, src_y, 40, 40),       // Size
    SDL2pp::Rect((int)pos_x, pos_y, 40, 40),  // Destination
    0.0,                                      // don't rotate
    SDL2pp::NullOpt,                          // rotation center - not needed
    flip                                      // horizontal flip
  );
}
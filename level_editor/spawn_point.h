#ifndef SPAWN_POINT_H_
#define SPAWN_POINT_H_

class SpawnPoint{
private:
  float pos_x;
  float pos_y;
  PositionConverter converter;
public:
  SpawnPoint(float posx, float posy) : pos_x(posx), pos_y(posy) {}

  float get_pos_x() {
    return this->pos_x;
  }

  float get_pos_y() {
    return this->pos_y;
  }

  void render(client_SDL& client_sdl, SDL2pp::Texture* spawn_texture) {
    SDL_Point pivot = {0, 0};
    int pos_x_px = converter.convert_from_m_to_px(pos_x);
    int pos_y_px = client_sdl.window.GetHeight() - converter.convert_from_m_to_px(pos_y);
      spawn_texture->SetAlphaMod(255);
      spawn_texture->SetBlendMode(SDL_BLENDMODE_BLEND);
      client_sdl.renderer.Copy(
          *spawn_texture, SDL2pp::Rect(0, 0, 60, 60), // Size
          SDL2pp::Rect(pos_x_px, 
                        pos_y_px,
                        60, 60),
                        0,
                        pivot, // rotation center - not needed
                        SDL_FLIP_NONE);
    }
};

#endif
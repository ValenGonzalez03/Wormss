#ifndef BEAM_H_
#define BEAM_H_

#include <SDL2pp/SDL2pp.hh>
#include "position_converter.h"
#include "client_SDL.h"

class Beam{
private:
  float pos_x;
  float pos_y;
  float angle;
  float length;
  PositionConverter converter;

public:
  Beam(float posx, float posy, float angle, float length) : 
    pos_x(posx), pos_y(posy), angle(angle), length(length) {}
  
  float get_pos_x() {
    return this->pos_x;
  }

  float get_pos_y() {
    return this->pos_y;
  }

  float get_angle() {
    return this->angle;
  }

  float get_length() {
    return this->length;
  }

  void render(client_SDL& client_sdl, SDL2pp::Texture* sb_text, SDL2pp::Texture* lb_text) {
    SDL_Point pivot = {0, 0};
    int pos_x_px = converter.convert_from_m_to_px(pos_x);
    int pos_y_px = client_sdl.window.GetHeight() - converter.convert_from_m_to_px(pos_y);
    if (length = 3.0) {
      sb_text->SetAlphaMod(255);
      int sbeam_height = sb_text->GetHeight();
      int sbeam_width = sb_text->GetWidth();
      client_sdl.renderer.Copy(
          *sb_text, SDL2pp::Rect(0, 0, sbeam_width - 10, sbeam_height), // Size
          SDL2pp::Rect(pos_x_px, 
                        pos_y_px,
                        sbeam_width + 20, sbeam_height),
                        -angle,
                        pivot, // rotation center - not needed
                        SDL_FLIP_NONE);
    }
    else if (length = 6.0) {
      lb_text->SetAlphaMod(255);
      int lbeam_height = lb_text->GetHeight();
      int lbeam_width = lb_text->GetWidth();
      client_sdl.renderer.Copy(
          *lb_text, SDL2pp::Rect(0, 0, lbeam_width - 10, lbeam_height), // Size
          SDL2pp::Rect(pos_x_px, 
                        pos_y_px,
                        lbeam_width + 20, lbeam_height),
                        -angle,
                        pivot, // rotation center - not needed
                        SDL_FLIP_NONE);
    }
    else {
      throw std::runtime_error("Error al renderizar la viga");
    }
  }


};

#endif
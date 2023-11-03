#ifndef CLIENT_BEAM_H_
#define CLIENT_BEAM_H_

#include <SDL2pp/SDL2pp.hh>
#include "client_position.h"

class Beam{
private:
    Position position;
    SDL2pp::Texture &texture;

public:
    Beam(Position &pos, SDL2pp::Texture &beam_texture) : position(pos), texture(beam_texture) {} 
};

#endif
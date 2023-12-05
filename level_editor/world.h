#ifndef WORLD_H_
#define WORLD_H_

#include "beam.h"
#include "spawn_point.h"
#include <string>
#include <vector>
#include <SDL2pp/SDL2pp.hh>
#include "client_SDL.h"

#define SHORT_BEAM "short_beam"
#define SHORT_BEAM_PATH "/Images/Weapons/grds4.png"

#define LONG_BEAM "long_beam"
#define LONG_BEAM_PATH "/Images/Weapons/grdl4.png"

#define WORM_WALKING "worm_walking"
#define WORM_WALKING_PATH "/Images/Worms/wwalk2.png"

class World {
private:
  std::string name = "";
  std::string background_name = "";
  std::vector<Beam> beams;
  std::vector<SpawnPoint> spawn_points;

  //SDL2pp::Renderer renderer;
  SDL2pp::Texture* short_beam_texture = nullptr;
  SDL2pp::Texture* long_beam_texture = nullptr;
  SDL2pp::Texture* worm_texture = nullptr;

  // Agrega la textura de short_beam
  void add_short_beam_texture(client_SDL& client_sdl);

  // Agrega la textura de long_beam
  void add_long_beam_texture(client_SDL& client_sdl);

  // Agrega la textura del gusano
  void add_worm_texture(client_SDL& client_sdl);

public:
  World();

  void initialize(client_SDL& client_sdl);

  void set_name(std::string new_name);

  void set_background_name (std::string new_backgorund_name);

  std::string get_name();

  std::string get_background_name();

  std::vector<Beam> get_beams();

  std::vector<SpawnPoint> get_spawn_points();

  void add_beam(float x, float y, float rotation, float length);

  void add_spawn_point(SpawnPoint spawn);

  void render(client_SDL& client_sdl);

  void delete_last_beam();

  void delete_last_spawn();

/*
  // Devuelve un ptr a la textura del short_beam
  SDL2pp::Texture* get_short_beam_texture();

  // Devuelve un ptr a la textura del long_beam
  SDL2pp::Texture* get_long_beam_texture();

  // Devuelve un ptr a la textura del worm caminando
  SDL2pp::Texture* get_worm_walking();
*/

  ~World();
};

#endif
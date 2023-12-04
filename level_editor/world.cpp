#include "world.h"

World::World() {}

void World::initialize(client_SDL& client_sdl){
  add_short_beam_texture(client_sdl);
  add_long_beam_texture(client_sdl);
  add_worm_texture(client_sdl);
}

void World::add_short_beam_texture(client_SDL& client_sdl){
  std::cout << "Agrego textura viga corta" << std::endl;
  SDL2pp::Surface src_surface = SDL2pp::Surface(std::string(RESOURCES_PATH) + SHORT_BEAM_PATH);
  SDL2pp::Texture* new_texture = new SDL2pp::Texture(client_sdl.renderer, src_surface);
  this->short_beam_texture = new_texture;
}

void World::add_long_beam_texture(client_SDL& client_sdl) {
  SDL2pp::Surface src_surface = SDL2pp::Surface(std::string(RESOURCES_PATH) + LONG_BEAM_PATH);
  SDL2pp::Texture* new_texture = new SDL2pp::Texture(client_sdl.renderer, src_surface);
  this->short_beam_texture = new_texture;
}

void World::add_worm_texture(client_SDL& client_sdl) {
  SDL2pp::Surface src_surface = SDL2pp::Surface(std::string(RESOURCES_PATH) + WORM_WALKING_PATH);
  SDL2pp::Texture* new_texture = new SDL2pp::Texture(client_sdl.renderer, src_surface);
  this->short_beam_texture = new_texture;
}

void World::set_name(std::string new_name) {
  this->name = new_name;
}

void World::set_background_name (std::string new_backgorund_name) {
  this->background_name = new_backgorund_name;
}

std::string World::get_name() {
  return this->name;
}

std::string World::get_background_name() {
  return this->background_name;
}

std::vector<Beam> World::get_beams() {
  return this->beams;
}

std::vector<SpawnPoint> World::get_spawn_points() {
  return this->spawn_points;
}

void World::add_beam(Beam beam) {
  this->beams.push_back(beam);
}

void World::add_spawn_point(SpawnPoint spawn) {
  this->spawn_points.push_back(spawn);
}

void World::render() {
  for (auto &beam : beams) {
    //beam.render(renderer);
    std::cout << "beam" << std::endl;
  }
  for (auto &spawn_point : spawn_points) {
    //spawn_point.render(renderer);
    std::cout << "spawn_point" << std::endl;
  }
}

SDL2pp::Texture* World::get_short_beam_texture() {
  return this->short_beam_texture;
}

SDL2pp::Texture* World::get_long_beam_texture() {
  return this->long_beam_texture;
}

SDL2pp::Texture* World::get_worm_walking() {
  return this->worm_texture;
}

World::~World() {
      if (short_beam_texture != nullptr) {
        delete short_beam_texture;
        short_beam_texture = nullptr;
    }

    if (long_beam_texture != nullptr) {
        delete long_beam_texture;
        long_beam_texture = nullptr;
    }

    if (worm_texture != nullptr) {
        delete worm_texture;
        worm_texture = nullptr;
    }
}
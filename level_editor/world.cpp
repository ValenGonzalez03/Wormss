#include "world.h"

World::World() {}

void World::initialize(client_SDL& client_sdl) {
  add_short_beam_texture(client_sdl);
  add_long_beam_texture(client_sdl);
  add_worm_texture(client_sdl);
}

void World::add_short_beam_texture(client_SDL& client_sdl) {
  SDL2pp::Surface src_surface = SDL2pp::Surface(std::string(RESOURCES_PATH) + SHORT_BEAM_PATH);
  this->short_beam_texture = new SDL2pp::Texture(client_sdl.renderer, src_surface);
}

void World::add_long_beam_texture(client_SDL& client_sdl) {
  SDL2pp::Surface src_surface = SDL2pp::Surface(std::string(RESOURCES_PATH) + LONG_BEAM_PATH);
  this->long_beam_texture = new SDL2pp::Texture(client_sdl.renderer, src_surface);
}

void World::add_worm_texture(client_SDL& client_sdl) {
  SDL2pp::Surface src_surface = SDL2pp::Surface(std::string(RESOURCES_PATH) + WORM_WALKING_PATH);
  this->worm_texture = new SDL2pp::Texture(client_sdl.renderer, src_surface);
}

void World::set_name(std::string new_name) { this->name = new_name; }

void World::set_background_name(std::string new_backgorund_name) { this->background_name = new_backgorund_name; }

std::string World::get_name() { return this->name; }

std::string World::get_background_name() { return this->background_name; }

std::vector<Beam> World::get_beams() { return this->beams; }

std::vector<SpawnPoint> World::get_spawn_points() { return this->spawn_points; }

void World::add_beam(float x, float y, float rotation, float length) {
  this->beams.emplace_back(x, y, rotation, length);
}

void World::add_spawn_point(SpawnPoint spawn) { this->spawn_points.push_back(spawn); }

void World::render(client_SDL& client_sdl) {
  for (auto& beam : beams) {
    beam.render(client_sdl, short_beam_texture, long_beam_texture);
  }
  for (auto& spawn_point : spawn_points) {
    spawn_point.render(client_sdl, worm_texture);
  }
}

void World::delete_last_beam() {
  if (beams.size() > 0) {
    beams.pop_back();
  }
}

void World::delete_last_spawn() {
  if (spawn_points.size() > 0) {
    spawn_points.pop_back();
  }
}

/*
SDL2pp::Texture* World::get_short_beam_texture() {
  return this->short_beam_texture;
}

SDL2pp::Texture* World::get_long_beam_texture() {
  return this->long_beam_texture;
}

SDL2pp::Texture* World::get_worm_walking() {
  return this->worm_texture;
}
*/

World::~World() {
  delete short_beam_texture;
  delete long_beam_texture;
  delete worm_texture;
}
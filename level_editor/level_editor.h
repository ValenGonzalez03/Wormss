#ifndef LEVEL_EDITOR_H_
#define LEVEL_EDITOR_H_

#include "world.h"
#include <sstream>

class LevelEditor {
 private:
  World& world;
  client_SDL client_sdl;
  bool placing_short_beam = false;
  bool placing_long_beam = false;
  bool placing_spawn_point = false;
  int beam_angle = 0;

  void handle_add_short_beam(int pos_x, int pos_y);

  void handle_add_long_beam(int pos_x, int pos_y);

  void handle_add_spawn_point(int pos_x, int pos_y);

  void handle_delete_beam();

  void handle_delete_spawn();

  void render_text();

 public:

  explicit LevelEditor(World& world);

  void run();

  bool execute_event(SDL_Event& event);

  void print_beams();

  void rotate_beam();
};

#endif
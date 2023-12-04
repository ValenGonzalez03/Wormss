#ifndef LEVEL_EDITOR_H_
#define LEVEL_EDITOR_H_

#include "world.h"

class LevelEditor {
private:
  World& world;
  //client_SDL client_sdl;

public:
  //explicit LevelEditor();

  explicit LevelEditor(World& world);

  void run();

  bool execute_event(SDL_Event &event);
};

#endif
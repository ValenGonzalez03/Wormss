#include "level_editor.h"

using namespace SDL2pp;

//LevelEditor::LevelEditor(){}

LevelEditor::LevelEditor(World& world) : world(world) {}

void LevelEditor::run() {
  // Initialize SDL library
  SDL sdl(SDL_INIT_VIDEO);

  // Initialize SDL_ttf library
  SDLTTF ttf;

  client_SDL client_sdl;

  world.initialize(client_sdl);

  SDL_Event event;

  while (true){
    if (execute_event(event)) // Si execute_event devuelve true, se
      return;                 // cierra el editor
      
    client_sdl.renderer.Clear();
    world.render(client_sdl);

    // Show rendered frame
    client_sdl.renderer.Present();

    SDL_Delay(16);
  }

}

bool LevelEditor::execute_event(SDL_Event& event) {
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) { // Cierra el juego
      //handle_finish_editor();
      return true;
    } else if (event.type == SDL_KEYDOWN) { // Aprieta una tecla
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
      case SDLK_q:
        //handle_finish_editor();
        return true;
      }
    }
    //return false;
  }
  return false;
}
#include "level_editor.h"

using namespace SDL2pp;

LevelEditor::LevelEditor(World& world) : world(world) {}

void LevelEditor::run() {
  // Initialize SDL library
  SDL sdl(SDL_INIT_VIDEO);

  // Initialize SDL_ttf library
  SDLTTF ttf;

  world.initialize(client_sdl);

  SDL_Event event;

  while (true) {
    if (execute_event(event))  // Si execute_event devuelve true, se
      return;                  // cierra el editor

    client_sdl.renderer.Clear();
    world.render(client_sdl);

    render_text();

    client_sdl.renderer.Present();

    SDL_Delay(16);
  }
}

bool LevelEditor::execute_event(SDL_Event& event) {
  PositionConverter converter;
  int mouse_x;
  int mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {  // Cierra el juego
      return true;
    } else if (event.type == SDL_KEYDOWN) {  // Aprieta una tecla
      switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
        case SDLK_q:
          return true;
        case SDLK_SPACE:  // Barra espaciadora
          rotate_beam();
          break;
        case SDLK_s:
          placing_short_beam = true;
          break;
        case SDLK_l:
          placing_long_beam = true;
          break;
        case SDLK_w:
          placing_spawn_point = true;
          break;
        case SDLK_z:
          handle_delete_beam();
          break;
        case SDLK_x:
          handle_delete_spawn();
          break;
      }
    } else if (event.type == SDL_KEYUP) {  // Suelta una tecla
      switch (event.key.keysym.sym) {
        case SDLK_s:
          if (placing_short_beam) {
            placing_short_beam = false;
          }
          break;
        case SDLK_l:
          if (placing_long_beam) {
            placing_long_beam = false;
          }
          break;
        case SDLK_w:
          if (placing_spawn_point) {
            placing_spawn_point = false;
          }
          break;
      }
    } else if (event.type == SDL_MOUSEBUTTONDOWN) {  // Hacer clic en el ratón
      if (placing_short_beam && event.button.button == SDL_BUTTON_LEFT) {
        handle_add_short_beam(mouse_x, mouse_y);
      } else if (placing_long_beam && event.button.button == SDL_BUTTON_LEFT) {
        handle_add_long_beam(mouse_x, mouse_y);
      } else if (placing_spawn_point && event.button.button == SDL_BUTTON_LEFT) {
        handle_add_spawn_point(mouse_x, mouse_y);
      }
    }
  }
  return false;
}


void LevelEditor::handle_add_short_beam(int pos_x, int pos_y) {
  PositionConverter converter;
  float pos_x_m = converter.convert_from_px_to_m(pos_x);
  float pos_y_m = converter.convert_from_px_to_m(client_sdl.window.GetHeight() - pos_y);
  world.add_beam(pos_x_m, pos_y_m, beam_angle, 3);
}

void LevelEditor::handle_add_long_beam(int pos_x, int pos_y) {
  PositionConverter converter;
  float pos_x_m = converter.convert_from_px_to_m(pos_x);
  float pos_y_m = converter.convert_from_px_to_m(client_sdl.window.GetHeight() - pos_y);
  world.add_beam(pos_x_m, pos_y_m, beam_angle, 6);
}

void LevelEditor::handle_add_spawn_point(int pos_x, int pos_y) {
  PositionConverter converter;
  float pos_x_m = converter.convert_from_px_to_m(pos_x);
  float pos_y_m = converter.convert_from_px_to_m(client_sdl.window.GetHeight() - pos_y);
  world.add_spawn_point(SpawnPoint(pos_x_m, pos_y_m));
}

void LevelEditor::handle_delete_beam() { world.delete_last_beam(); }

void LevelEditor::handle_delete_spawn() { world.delete_last_spawn(); }


void LevelEditor::print_beams() {
  std::vector<Beam> beams = world.get_beams();
  int counter = 0;
  for (auto& beam : beams) {
    std::cout << counter << "beam: " << beam.get_length() << std::endl;
    counter++;
  }
}

void LevelEditor::rotate_beam() {
  beam_angle += 5.0;
  if (beam_angle >= 180.0) {
    beam_angle = 0.0;
  }
}

void LevelEditor::render_text() {
  std::string text =
      "Viga corta: s + click izquierdo\n"
      "Viga larga: l + click izquierdo\n"
      "Spawn: w + click izquierdo\n"
      "Cambiar angulo: tecla de espacio\n"
      "Eliminar viga: z\n"
      "Eliminar spawn: x\n"
      "Angulo viga: " +
      std::to_string(beam_angle);

  Font font(RESOURCES_PATH "/Vera.ttf", 15);

  std::istringstream textStream(text);
  std::string line;
  int yOffset = 0;

  while (std::getline(textStream, line, '\n')) {
    Texture text_sprite(client_sdl.renderer, (font).RenderText_Blended(line, SDL_Color{255, 255, 255, 255}));

    client_sdl.renderer.Copy(text_sprite, NullOpt, Rect(0, yOffset, text_sprite.GetWidth(), text_sprite.GetHeight()));

    yOffset += text_sprite.GetHeight();
  }
}
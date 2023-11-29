#ifndef CLIENT_RESOURCE_POOL_H_
#define CLIENT_RESOURCE_POOL_H_

#include <SDL2pp/SDL2pp.hh>
#include <map>
#include <memory>
#include <string>

#define SHORT_BEAM "short_beam"
#define SHORT_BEAM_PATH "/Images/Weapons/grds4.png"

#define LONG_BEAM "long_beam"
#define LONG_BEAM_PATH "/Images/Weapons/grdl4.png"

#define WORM_WALKING "worm_walking"
#define WORM_WALKING_PATH "/Images/Worms/wwalk2.png"

#define WORM_JUMPING "worm_jumping"
#define WORM_JUMPING_PATH "/Images/Worms/wjump.png"

class ResourcePool {
private:
  SDL2pp::Renderer &renderer;
  std::map<std::string, std::shared_ptr<SDL2pp::Texture>> textures;
  std::map<std::string, std::shared_ptr<SDL2pp::Font>> fonts;
  std::map<std::string, Mix_Chunk*> sounds;
  std::shared_ptr<SDL2pp::Texture> background;
  Mix_Music* gMusic = NULL;

  void add_texture(const std::string &texture_name,
                   const std::string &image_path) {
    // Aca estoy trabajando con la version no wrappeada de SDL2, porque
    // sinceramente no pude encontrar otra forma de hacerlo.
    SDL2pp::Surface surface = SDL2pp::Surface(RESOURCES_PATH + image_path);
    Uint32 color_key = SDL_MapRGB(surface.Get()->format, 128, 128, 192);

    textures[texture_name] = std::make_shared<SDL2pp::Texture>(
        renderer, surface.SetColorKey(
                      true, color_key)); // Esta color_key es especifica para
                                         // los fondos de los gusanos.
    // Para imagenes con otros fondos no haria efecto, por lo que si hay
    // distintas habria que poner ifs.
  }

  // Devuelve la textura guardada en el map de texturas
  // relacionada con el texture_name
  SDL2pp::Texture *get_texture(const std::string &texture_name) {
    try {
      return textures.at(texture_name).get();
    } catch (const std::out_of_range &e) {
      throw std::runtime_error("Texture not found: " + texture_name);
    }
  }

  // Agrega la textura de short_beam al map de texturas
  void add_short_beam() { add_texture(SHORT_BEAM, SHORT_BEAM_PATH); }

  // Agrega la textura de long_beam al map de texturas
  void add_long_beam() { add_texture(LONG_BEAM, LONG_BEAM_PATH); }

  // Agrega la textura del gusano caminando al map de texturas
  void add_worm_walking() { add_texture(WORM_WALKING, WORM_WALKING_PATH); }

  void add_worm_jumping() { add_texture(WORM_JUMPING, WORM_JUMPING_PATH); }

  

public:

  // Crea la Resource pool y le carga las texturas
  explicit ResourcePool(SDL2pp::Renderer &rend) : renderer(rend) {}

  void initialize () {
    add_short_beam();
    add_long_beam();
    add_worm_walking();
    add_worm_jumping();
    //add_font("Vera20", "/Vera.ttf", 20);
    //add_font("Vera12", "/Vera.ttf", 12);
  }

  // Devuelve un ptr a la textura del short_beam
  SDL2pp::Texture *get_short_beam_texture() { return get_texture(SHORT_BEAM); }

  // Devuelve un ptr a la textura del long_beam
  SDL2pp::Texture *get_long_beam_texture() { return get_texture(LONG_BEAM); }

  // Devuelve un ptr a la textura del worm caminando
  SDL2pp::Texture *get_worm_walking() { return get_texture(WORM_WALKING); }

/*
  void add_sound(const std::string sound_name, const std::string &sound_path) {
    Mix_Chunk * 
  }
*/

  // Ver inicialización (NO USAR)
  void add_font(const std::string &font_name, const std::string &font_path,
                int font_size) {
    try {
    SDL2pp::Font font(RESOURCES_PATH + font_path, font_size);
    this->fonts[font_name] = std::make_shared<SDL2pp::Font>(std::move(font));
    } catch(const std::exception &err) {
      std::cout << "Error al agregar la fuente: " << font_name << std::endl;
    }
  }

  // Ver inicialización (NO USAR)
  std::shared_ptr<SDL2pp::Font> get_font(const std::string &font_name) {
    auto it = this->fonts.find(font_name);
    if (it == this->fonts.end()) {
      throw std::runtime_error("Font not found: " + font_name);
    }
    return it->second;
  }

  void add_background(const std::string &image_path) {
    std::cout << "add_background" << std::endl;
    SDL2pp::Surface surface = SDL2pp::Surface(RESOURCES_PATH + image_path);
    Uint32 color_key = SDL_MapRGB(surface.Get()->format, 128, 128, 192);

    background = std::make_shared<SDL2pp::Texture>(
        renderer, surface.SetColorKey(
                      true, color_key)); // Esta color_key es especifica para
                                         // los fondos de los gusanos.
    // Para imagenes con otros fondos no haria efecto, por lo que si hay
    // distintas habria que poner ifs.
  }

  std::shared_ptr<SDL2pp::Texture> get_background() {
    try {
      std::cout << "get_background" << std::endl;
      return background;
    } catch (const std::exception &e) {
      throw std::runtime_error("Background not found.");
    }
  }


  ~ResourcePool() {}


};

#endif
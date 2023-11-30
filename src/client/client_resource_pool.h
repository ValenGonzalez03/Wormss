#ifndef CLIENT_RESOURCE_POOL_H_
#define CLIENT_RESOURCE_POOL_H_

#include <SDL2pp/SDL2pp.hh>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

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
  std::map<std::string, std::vector<SDL2pp::Texture *>> texture_arrays;
  std::map<std::string, std::shared_ptr<SDL2pp::Font>> fonts;
  std::map<std::string, Mix_Chunk*> sounds;
  std::shared_ptr<SDL2pp::Texture> background;
  Mix_Music* gMusic = NULL;
  unsigned int music_volume = 20;

  void add_texture(const std::string &texture_name,
                   const std::string &image_path, int width, int height,
                   int amount_frames) {
    SDL2pp::Surface src_surface = SDL2pp::Surface(RESOURCES_PATH + image_path);
    std::vector<SDL2pp::Texture *> textures(amount_frames);
    for (int i = 0; i < amount_frames; i++) {
      SDL2pp::Rect src_rect(0, -i * height, width, height);
      SDL2pp::Surface dst_surface(0, src_rect.w, src_rect.h, 32, 0, 0, 0, 0);
      src_surface.Blit(SDL2pp::NullOpt, dst_surface, src_rect);

      Uint32 color_key = SDL_MapRGB(dst_surface.Get()->format, 128, 128, 192);
      SDL2pp::Texture *texture(new SDL2pp::Texture(
          renderer, dst_surface.SetColorKey(true, color_key)));
      textures[i] = texture;
    }
    texture_arrays[texture_name] = textures;
  }

  // Devuelve la textura guardada en el map de texturas
  // relacionada con el texture_name
  std::vector<SDL2pp::Texture *> get_texture(const std::string &texture_name) {
    try {
      return texture_arrays.at(texture_name);
    } catch (const std::out_of_range &e) {
      throw std::runtime_error("Texture not found: " + texture_name);
    }
  }

  // Agrega la textura de short_beam al map de texturas
  void add_short_beam() { add_texture(SHORT_BEAM, SHORT_BEAM_PATH, 72, 20, 1); }

  // Agrega la textura de long_beam al map de texturas
  void add_long_beam() { add_texture(LONG_BEAM, LONG_BEAM_PATH, 140, 20, 1); }

  // Agrega la textura del gusano caminando al map de texturas
  void add_worm_walking() {
    add_texture(WORM_WALKING, WORM_WALKING_PATH, 60, 60, 15);
  }

  void add_worm_jumping() {
    add_texture(WORM_JUMPING, WORM_JUMPING_PATH, 60, 60, 10);
  }

  

public:
  // Crea la Resource pool y le carga las texturas
  explicit ResourcePool(SDL2pp::Renderer &rend) : renderer(rend) {}

  void initialize () {
    add_short_beam();
    add_long_beam();
    add_worm_walking();
    add_worm_jumping();
    add_music(std::string(RESOURCES_PATH) + "/Sounds/music.wav");
    //add_font("Vera20", "/Vera.ttf", 20);
    //add_font("Vera12", "/Vera.ttf", 12);
  }

  // Devuelve un ptr a la textura del short_beam
  std::vector<SDL2pp::Texture *> get_short_beam_texture() {
    return get_texture(SHORT_BEAM);
  }

  // Devuelve un ptr a la textura del long_beam
  std::vector<SDL2pp::Texture *> get_long_beam_texture() {
    return get_texture(LONG_BEAM);
  }

  // Devuelve un ptr a la textura del worm caminando
  std::vector<SDL2pp::Texture *> get_worm_walking() {
    return get_texture(WORM_WALKING);
  }

  /*
    void add_sound(const std::string sound_name, const std::string &sound_path)
    { Mix_Chunk *
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
                      true, color_key));
  }

  std::shared_ptr<SDL2pp::Texture> get_background() {
    try {
      std::cout << "get_background" << std::endl;
      return background;
    } catch (const std::exception &e) {
      throw std::runtime_error("Background not found.");
    }
  }

  //Load music
  void add_music(std::string absolute_path) {
    gMusic = Mix_LoadMUS( (absolute_path).c_str() );
    if( gMusic == NULL ) {
      throw std::runtime_error( "Failed to load music!: Incorrect path");
    }
  }

  void play_music() {
    if( gMusic == NULL ) {
      throw std::runtime_error( "Failed to play music!: No music loaded");
    }
    Mix_PlayMusic(gMusic, -1);
    Mix_VolumeMusic(music_volume);
  }

  void turn_music_volume_down() {
    if (music_volume >= 10) {
      music_volume -= 10;
      Mix_VolumeMusic(music_volume);
    }
  }

  void turn_music_volume_up() {
    if (music_volume <= 110) {
      music_volume += 10;
      Mix_VolumeMusic(music_volume);
    }
  }


  ~ResourcePool() {}


};

#endif
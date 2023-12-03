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
                   int amount_frames);

  // Devuelve la textura guardada en el map de texturas
  // relacionada con el texture_name
  std::vector<SDL2pp::Texture *> get_texture(const std::string &texture_name);

  // Agrega la textura de short_beam al map de texturas
  void add_short_beam();

  // Agrega la textura de long_beam al map de texturas
  void add_long_beam();

  // Agrega la textura del gusano caminando al map de texturas
  void add_worm_walking();

  // Agrega la textura del gusano saltando al map de texturas
  void add_worm_jumping();

public:
  // Crea la Resource pool y le carga las texturas
  explicit ResourcePool(SDL2pp::Renderer &rend);

  // Inicializa la resource pool (carga las texturas)
  void initialize ();

  // Devuelve un ptr a la textura del short_beam
  std::vector<SDL2pp::Texture *> get_short_beam_texture();

  // Devuelve un ptr a la textura del long_beam
  std::vector<SDL2pp::Texture *> get_long_beam_texture();

  // Devuelve un ptr a la textura del worm caminando
  std::vector<SDL2pp::Texture *> get_worm_walking();

  
  /*
    void add_sound(const std::string sound_name, const std::string &sound_path)
    { Mix_Chunk *
    }
  */

  // Ver inicialización (NO USAR)
  void add_font(const std::string &font_name, const std::string &font_path,
                int font_size);

  // Ver inicialización (NO USAR)
  std::shared_ptr<SDL2pp::Font> get_font(const std::string &font_name);

  void add_background(const std::string &image_path);

  std::shared_ptr<SDL2pp::Texture> get_background();

  //Load music
  void add_music(std::string absolute_path);

  void play_music();

  void turn_music_volume_down();

  void turn_music_volume_up();

  ~ResourcePool();

};

#endif
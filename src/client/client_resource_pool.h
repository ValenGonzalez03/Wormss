#ifndef CLIENT_RESOURCE_POOL_H_
#define CLIENT_RESOURCE_POOL_H_

#include <SDL2pp/SDL2pp.hh>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "../common/game_constants.h"

enum BACKGROUND_COLOR { LIGHT_BLUE, YELLOW };

class ResourcePool {
 private:
  SDL2pp::Renderer &renderer;
  std::map<std::string, std::vector<SDL2pp::Texture *>> texture_arrays;
  std::map<std::string, std::shared_ptr<SDL2pp::Font>> fonts;
  std::map<std::string, Mix_Chunk *> sounds;
  std::shared_ptr<SDL2pp::Texture> background;
  Mix_Music *gMusic = NULL;
  unsigned int music_volume = 20;

  void add_texture(const std::string &texture_name, const std::string &image_path,
                   int width, int height, int amount_frames, BACKGROUND_COLOR back_color,
                   int offset_x = 0, int offset_y = 0, int offset_width = 0,
                   int offset_height = 0, bool textures_not_centered = false);

  void add_texture_walking(const std::string &texture_name, const std::string &image_path,
                           int width, int height, int amount_frames);

  void add_texture_aiming(const std::string &texture_name, const std::string &image_path,
                          int width, int height, int amount_frames);

  // Devuelve la textura guardada en el map de texturas
  // relacionada con el texture_name
  std::vector<SDL2pp::Texture *> get_texture(const std::string &texture_name) const;

  // Agrega la textura de short_beam al map de texturas
  void add_short_beam();

  // Agrega la textura de long_beam al map de texturas
  void add_long_beam();

  // Agrega las texturas del gusano caminando al map de texturas
  void add_worm_walking();

  // Agrega las texturas del gusano saltando al map de texturas
  void add_worm_jumping();

  // Agrega las texturas del gusano apuntando con todas las armas al map de texturas
  void add_worm_aiming();

  // Agrega las texturas del gusano atacando con todas las armas al map de texturas
  void add_worm_attacking();

  // Agrega la textura de los explodables al map de texturas
  void add_explodables_textures();

 public:
  // Crea la Resource pool y le carga las texturas
  explicit ResourcePool(SDL2pp::Renderer &rend);  // NOLINT(runtime/references)

  // Inicializa la resource pool (carga las texturas)
  void initialize();

  // Devuelve un ptr a la textura del short_beam
  std::vector<SDL2pp::Texture *> get_short_beam_texture() const;

  // Devuelve un ptr a la textura del long_beam
  std::vector<SDL2pp::Texture *> get_long_beam_texture() const;

  // Devuelve un ptr a la textura del worm caminando
  std::vector<SDL2pp::Texture *> get_worm_walking() const;

  // Devuelve un ptr a la textura del worm saltando
  std::vector<SDL2pp::Texture *> get_worm_jumping() const;

  // Devuelve un ptr a la textura del worm apuntando dependiendo del arma que este utilizando en ese momento
  std::vector<SDL2pp::Texture *> get_worm_aiming(WeaponType type) const;

  // Devuelve un ptr a la textura del worm atacando dependiendo del arma que este utilizando en ese momento
  std::vector<SDL2pp::Texture *> get_worm_attacking(WeaponType type) const;

  // Devuelve un ptr a la textura del misil de la bazooka
  std::vector<SDL2pp::Texture *> get_missile_texture() const;

  // Devuelve un ptr a la textura de la granada
  std::vector<SDL2pp::Texture *> get_grenade_texture() const;
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

  std::shared_ptr<SDL2pp::Texture> get_background() const;

  // Load music
  void add_music(const std::string &absolute_path);

  void play_music();

  void turn_music_volume_down();

  void turn_music_volume_up();

  ~ResourcePool();
};

#endif

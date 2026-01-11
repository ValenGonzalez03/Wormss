#include "client_resource_pool.h"

#define SHORT_BEAM "short_beam"
#define SHORT_BEAM_PATH "/Images/Weapons/grds4.png"

#define LONG_BEAM "long_beam"
#define LONG_BEAM_PATH "/Images/Weapons/grdl4.png"

#define WORM_WALKING "worm_walking"
#define WORM_WALKING_PATH "/Images/Worms/wwalk2.png"

#define WORM_JUMPING "worm_jumping"
#define WORM_JUMPING_PATH "/Images/Worms/wjump.png"

#define WORM_AIMING_BAZ "worm_aiming_baz"
#define WORM_AIMING_BAZ_PATH "/Images/Worms/wbaz.png"

#define WORM_AIMING_BAT "worm_aiming_bat"
#define WORM_AIMING_BAT_PATH "/Images/Worms/wbsbaim.png"

#define WORM_ATTACKING_BAT "worm_attacking_bat"
#define WORM_ATTACKING_BAT_PATH "/Images/Worms/wbsbswn.png"

#define WORM_AIMING_GRN "worm_aiming_grn"
#define WORM_AIMING_GRN_PATH "/Images/Worms/wthrgrn.png"

#define MISSILE "missile"
#define MISSILE_PATH "/Images/Weapons/missile.png"

#define GRENADE_TX "grenade"
#define GRENADE_TX_PATH "/Images/Weapons/grenade.png"

ResourcePool::ResourcePool(SDL2pp::Renderer &rend) : renderer(rend) {}

void ResourcePool::initialize() {
  add_short_beam();
  add_long_beam();
  add_worm_walking();
  add_worm_jumping();
  add_worm_aiming();
  add_worm_attacking();
  add_explodables_textures();
  add_music(std::string(RESOURCES_PATH) + "/Sounds/music.wav");
  //add_font("Vera20", "/Vera.ttf", 20);
  //add_font("Vera12", "/Vera.ttf", 12);
}

void ResourcePool::add_texture(const std::string &texture_name,
                   const std::string &image_path, int width, int height,
                   int amount_frames, BACKGROUND_COLOR back_color, int offset_x, int offset_y, int offset_width, int offset_height, 
                   bool textures_not_centered) {
  SDL2pp::Surface src_surface = SDL2pp::Surface(RESOURCES_PATH + image_path);
  std::vector<SDL2pp::Texture *> textures(amount_frames);

  for (int i = 0; i < amount_frames; i++) {
    SDL2pp::Rect src_rect(0, i * height, width, height);
    SDL2pp::Surface int_surface(0, src_rect.GetW(), src_rect.GetH(), 32, 0, 0, 0, 0);
    src_surface.BlitScaled(src_rect, int_surface, SDL2pp::NullOpt);

    //if (!(offset_x == 0 && offset_y == 0 && offset_width == 0 && offset_height == 0)) {
    textures_not_centered = static_cast<int>(textures_not_centered);

    SDL2pp::Rect dst_rect(offset_x - textures_not_centered * (i / 3), offset_y, 
    width + offset_width + textures_not_centered * (i / 3), height + offset_height);
    SDL2pp::Surface dst_surface(0, dst_rect.GetW(), dst_rect.GetH(), 32, 0, 0, 0, 0);
    int_surface.BlitScaled(dst_rect, dst_surface, SDL2pp::NullOpt);
    //}

    Uint32 color_key;
    if (back_color == LIGHT_BLUE)
      color_key = SDL_MapRGB(dst_surface.Get()->format, 128, 128, 192);
    else if (back_color == YELLOW)
      color_key = SDL_MapRGB(dst_surface.Get()->format, 192, 192, 128);
    
    SDL2pp::Texture *texture(new SDL2pp::Texture(renderer, dst_surface.SetColorKey(true, color_key)));
    textures[i] = texture;
  }
  texture_arrays[texture_name] = textures;
}

std::vector<SDL2pp::Texture* > ResourcePool::get_texture(const std::string &texture_name)  {
  try {
    return texture_arrays.at(texture_name);
  } catch (const std::out_of_range &e) {
    throw std::runtime_error("Texture not found: " + texture_name);
  }
}

// ==================================== AGREGADO DE TEXTURAS ==================================== //
// ============================================================================================== //

void ResourcePool::add_background(const std::string &image_path) {
  SDL2pp::Surface surface = SDL2pp::Surface(std::string(RESOURCES_PATH) + "/Images/Backgrounds/" + image_path);
  Uint32 color_key = SDL_MapRGB(surface.Get()->format, 128, 128, 192);

  background = std::make_shared<SDL2pp::Texture>(renderer, surface.SetColorKey(true, color_key));
}

void ResourcePool::add_short_beam() { 
  add_texture(SHORT_BEAM, SHORT_BEAM_PATH, 72, 20, 1, LIGHT_BLUE);
}

void ResourcePool::add_long_beam() { 
  add_texture(LONG_BEAM, LONG_BEAM_PATH, 140, 20, 1, LIGHT_BLUE);
}

void ResourcePool::add_worm_walking() {
  add_texture(WORM_WALKING, WORM_WALKING_PATH, 60, 60, 15, LIGHT_BLUE, 19, 13, -38, -30, true);
}

void ResourcePool::add_worm_jumping() {
  add_texture(WORM_JUMPING, WORM_JUMPING_PATH, 60, 60, 10, LIGHT_BLUE);
}

void ResourcePool::add_worm_aiming() {
  add_texture(WORM_AIMING_BAZ, WORM_AIMING_BAZ_PATH, 60, 60, 32, LIGHT_BLUE, 16, 14, -29, -31, false);
  add_texture(WORM_AIMING_BAT, WORM_AIMING_BAT_PATH, 60, 60, 32, YELLOW, 15, 0, -15, 0, false);
  add_texture(WORM_AIMING_GRN, WORM_AIMING_GRN_PATH, 60, 60, 32, LIGHT_BLUE, 16, 12, -29, -28, false);
}

void ResourcePool::add_worm_attacking() {
  add_texture(WORM_ATTACKING_BAT, WORM_ATTACKING_BAT_PATH, 60, 60, 32, YELLOW, 0, 0, -16, 0, false);
}

void ResourcePool::add_explodables_textures() {
  add_texture(MISSILE, MISSILE_PATH, 60, 60, 32, LIGHT_BLUE, 17, 23, -35, -46, false);
  add_texture(GRENADE_TX, GRENADE_TX_PATH, 60, 60, 32, LIGHT_BLUE, 24, 16, -48, -38, false);
}

// ============================================================================================== //
// ============================================================================================== //


// ======================================= GET DE TEXTURAS ==================================== //
// ============================================================================================ //

std::shared_ptr<SDL2pp::Texture> ResourcePool::get_background() {
  try {
    return background;
  } catch (const std::exception &e) {
    throw std::runtime_error("Background not found.");
  }
}

std::vector<SDL2pp::Texture *> ResourcePool::get_short_beam_texture() {
  return get_texture(SHORT_BEAM);
}

std::vector<SDL2pp::Texture *> ResourcePool::get_long_beam_texture() {
  return get_texture(LONG_BEAM);
}

std::vector<SDL2pp::Texture *> ResourcePool::get_worm_walking() {
  return get_texture(WORM_WALKING);
}

std::vector<SDL2pp::Texture *> ResourcePool::get_worm_jumping() {
  return get_texture(WORM_JUMPING);
}

std::vector<SDL2pp::Texture *> ResourcePool::get_worm_aiming(WeaponType type) {
  switch (type)
  {
  case BAZOOKA:
    return get_texture(WORM_AIMING_BAZ);
  case BAT:
    return get_texture(WORM_AIMING_BAT);
  case GRENADE:
    return get_texture(WORM_AIMING_GRN);
  default:
    throw std::runtime_error("Invalid weapon type for aiming texture");
    break;
  }
}

std::vector<SDL2pp::Texture *> ResourcePool::get_worm_attacking(WeaponType type) {
  switch (type)
  {
  case BAZOOKA:
    return get_texture(WORM_AIMING_BAZ);
  case BAT:
    return get_texture(WORM_ATTACKING_BAT);
  default:
    throw std::runtime_error("Invalid weapon type for aiming texture");
    break;
  }
}

std::vector<SDL2pp::Texture *> ResourcePool::get_missile_texture() {
  return get_texture(MISSILE);
}

std::vector<SDL2pp::Texture *> ResourcePool::get_grenade_texture() {
  return get_texture(GRENADE_TX);
}
// ============================================================================================== //
// ============================================================================================== //


// ======================================= OTROS ==================================== //
// ================================================================================== //

void ResourcePool::add_font(const std::string &font_name, const std::string &font_path,
              int font_size) {
  try {
  SDL2pp::Font font(RESOURCES_PATH + font_path, font_size);
  this->fonts[font_name] = std::make_shared<SDL2pp::Font>(std::move(font));
  } catch(const std::exception &err) {
    std::cout << "Error al agregar la fuente: " << font_name << std::endl;
  }
}

std::shared_ptr<SDL2pp::Font> ResourcePool::get_font(const std::string &font_name) {
  auto it = this->fonts.find(font_name);
  if (it == this->fonts.end()) {
    throw std::runtime_error("Font not found: " + font_name);
  }
  return it->second;
}

void ResourcePool::add_music(std::string absolute_path) {
  gMusic = Mix_LoadMUS( (absolute_path).c_str() );
  if( gMusic == NULL ) {
    throw std::runtime_error( "Failed to load music!: Incorrect path");
  }
}

void ResourcePool::play_music() {
  if( gMusic == NULL ) {
    throw std::runtime_error( "Failed to play music!: No music loaded");
  }
  Mix_PlayMusic(gMusic, -1);
  Mix_VolumeMusic(music_volume);
}

void ResourcePool::turn_music_volume_down() {
  if (music_volume >= 10) {
    music_volume -= 10;
    Mix_VolumeMusic(music_volume);
  }
}

void ResourcePool::turn_music_volume_up() {
  if (music_volume <= 110) {
    music_volume += 10;
    Mix_VolumeMusic(music_volume);
  }
}

// ================================================================================== //
// ================================================================================== //

ResourcePool::~ResourcePool() {
  // Elimino las texturas
  for (auto& pair : texture_arrays) {
    std::vector<SDL2pp::Texture*>& textures = pair.second;
    for (SDL2pp::Texture* texture : textures) {
      delete texture;
    }
    textures.clear();
  }
  texture_arrays.clear();
}
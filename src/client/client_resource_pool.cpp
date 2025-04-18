#include "client_resource_pool.h"

ResourcePool::ResourcePool(SDL2pp::Renderer &rend) : renderer(rend) {}

void ResourcePool::initialize() {
  add_short_beam();
  add_long_beam();
  add_worm_walking();
  add_worm_jumping();
  add_worm_aiming();
  add_music(std::string(RESOURCES_PATH) + "/Sounds/music.wav");
  //add_font("Vera20", "/Vera.ttf", 20);
  //add_font("Vera12", "/Vera.ttf", 12);
}

void ResourcePool::add_texture(const std::string &texture_name,
                   const std::string &image_path, int width, int height,
                   int amount_frames)  {
  SDL2pp::Surface src_surface = SDL2pp::Surface(RESOURCES_PATH + image_path);
  std::vector<SDL2pp::Texture *> textures(amount_frames);
  for (int i = 0; i < amount_frames; i++) {
    SDL2pp::Rect src_rect(0, -i * height, width, height);
    SDL2pp::Surface dst_surface(0, src_rect.GetW(), src_rect.GetH(), 32, 0, 0, 0, 0);
    src_surface.Blit(SDL2pp::NullOpt, dst_surface, src_rect);

    Uint32 color_key = SDL_MapRGB(dst_surface.Get()->format, 128, 128, 192);
    SDL2pp::Texture *texture(new SDL2pp::Texture(
        renderer, dst_surface.SetColorKey(true, color_key)));
    textures[i] = texture;
  }
  texture_arrays[texture_name] = textures;
}

void ResourcePool::add_texture_walking(const std::string &texture_name,
  const std::string &image_path, int width, int height,
  int amount_frames)  {
SDL2pp::Surface src_surface = SDL2pp::Surface(RESOURCES_PATH + image_path);
std::vector<SDL2pp::Texture *> textures(amount_frames);
for (int i = 0; i < amount_frames; i++) {
  SDL2pp::Rect src_rect(0, i * height, width, height);
  SDL2pp::Surface int_surface(0, src_rect.GetW(), src_rect.GetH(), 32, 0, 0, 0, 0);
  src_surface.BlitScaled(src_rect, int_surface, SDL2pp::NullOpt);
  
  const int offset_x = 19;
  const int offset_y = 14;
  SDL2pp::Rect dst_rect(offset_x - i / 3, offset_y, width + offset_x + i / 3 - 57, height + offset_y - 45);
  SDL2pp::Surface dst_surface(0, dst_rect.GetW(), dst_rect.GetH(), 32, 0, 0, 0, 0);
  int_surface.BlitScaled(dst_rect, dst_surface, SDL2pp::NullOpt);
  //SDL2pp::Surface s();

  Uint32 color_key = SDL_MapRGB(dst_surface.Get()->format, 128, 128, 192);
  SDL2pp::Texture *texture(new SDL2pp::Texture(
  renderer, dst_surface.SetColorKey(true, color_key)));
  textures[i] = texture;
}
texture_arrays[texture_name] = textures;
}

void ResourcePool::add_texture_aiming(const std::string &texture_name,
  const std::string &image_path, int width, int height,
  int amount_frames)  {
SDL2pp::Surface src_surface = SDL2pp::Surface(RESOURCES_PATH + image_path);
std::vector<SDL2pp::Texture *> textures(amount_frames);
for (int i = 0; i < amount_frames; i++) {
  SDL2pp::Rect src_rect(0, i * height, width, height);
  SDL2pp::Surface int_surface(0, src_rect.GetW(), src_rect.GetH(), 32, 0, 0, 0, 0);
  src_surface.BlitScaled(src_rect, int_surface, SDL2pp::NullOpt);
  
  const int offset_x = 17;
  const int offset_y = 14;
  SDL2pp::Rect dst_rect(offset_x, offset_y, width + offset_x - 48, height + offset_y - 45);
  SDL2pp::Surface dst_surface(0, dst_rect.GetW(), dst_rect.GetH(), 32, 0, 0, 0, 0);
  int_surface.BlitScaled(dst_rect, dst_surface, SDL2pp::NullOpt);
  //SDL2pp::Surface s();

  Uint32 color_key = SDL_MapRGB(dst_surface.Get()->format, 128, 128, 192);
  SDL2pp::Texture *texture(new SDL2pp::Texture(
  renderer, dst_surface.SetColorKey(true, color_key)));
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


void ResourcePool::add_short_beam() { 
  add_texture(SHORT_BEAM, SHORT_BEAM_PATH, 72, 20, 1);
}

std::vector<SDL2pp::Texture *> ResourcePool::get_short_beam_texture() {
  return get_texture(SHORT_BEAM);
}

void ResourcePool::add_long_beam() { 
  add_texture(LONG_BEAM, LONG_BEAM_PATH, 140, 20, 1);
}

std::vector<SDL2pp::Texture *> ResourcePool::get_long_beam_texture() {
  return get_texture(LONG_BEAM);
}

void ResourcePool::add_worm_walking() {
  add_texture_walking(WORM_WALKING, WORM_WALKING_PATH, 60, 60, 15);
}

std::vector<SDL2pp::Texture *> ResourcePool::get_worm_walking() {
  return get_texture(WORM_WALKING);
}

void ResourcePool::add_worm_jumping() {
  add_texture(WORM_JUMPING, WORM_JUMPING_PATH, 60, 60, 10);
}

std::vector<SDL2pp::Texture *> ResourcePool::get_worm_jumping() {
  return get_texture(WORM_JUMPING);
}

void ResourcePool::add_worm_aiming() {
  add_texture_aiming(WORM_AIMING, WORM_AIMING_PATH, 60, 60, 32);
}

std::vector<SDL2pp::Texture *> ResourcePool::get_worm_aiming() {
  return get_texture(WORM_AIMING);
}

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

void ResourcePool::add_background(const std::string &image_path) {
  SDL2pp::Surface surface = SDL2pp::Surface(std::string(RESOURCES_PATH) + "/Images/Backgrounds/" + image_path);
  Uint32 color_key = SDL_MapRGB(surface.Get()->format, 128, 128, 192);

  background = std::make_shared<SDL2pp::Texture>(
      renderer, surface.SetColorKey(
                    true, color_key));
}

std::shared_ptr<SDL2pp::Texture> ResourcePool::get_background() {
  try {
    return background;
  } catch (const std::exception &e) {
    throw std::runtime_error("Background not found.");
  }
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
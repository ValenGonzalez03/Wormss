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
#define WORM_WALKING_PATH "/Images/Worms/wwalk.png"

class ResourcePool {
private:
  SDL2pp::Renderer &renderer;
  std::map<std::string, std::shared_ptr<SDL2pp::Texture>> textures;

  void add_texture(std::string texture_name, std::string image_path) {
    textures[texture_name] = std::make_shared<SDL2pp::Texture>(
        renderer,
        SDL2pp::Surface(RESOURCES_PATH + image_path).SetColorKey(true, 0));
  }

  SDL2pp::Texture *get_texture(const std::string &texture_name) {
    try {
      return textures.at(texture_name).get();
    } catch (const std::out_of_range &e) {
      throw std::runtime_error("Texture not found: " + texture_name);
    }
  }

  void add_short_beam() { add_texture(SHORT_BEAM, SHORT_BEAM_PATH); }

  void add_long_beam() { add_texture(LONG_BEAM, LONG_BEAM_PATH); }

  void add_worm_walking() { add_texture(WORM_WALKING, WORM_WALKING_PATH); }

public:
  explicit ResourcePool(SDL2pp::Renderer &rend) : renderer(rend) {
    add_short_beam();
    add_long_beam();
    add_worm_walking();
  }

  SDL2pp::Texture *get_short_beam_texture() { return get_texture(SHORT_BEAM); }

  SDL2pp::Texture *get_long_beam_texture() { return get_texture(LONG_BEAM); }

  SDL2pp::Texture *get_worm_walking() { return get_texture(WORM_WALKING); }
};

#endif
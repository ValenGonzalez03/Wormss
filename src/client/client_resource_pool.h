#ifndef CLIENT_RESOURCE_POOL_H_
#define CLIENT_RESOURCE_POOL_H_

#include <SDL2pp/SDL2pp.hh>
#include <map>
#include <string>
#include <memory>

#define SHORT_BEAM "short_beam"
#define SHORT_BEAM_PATH "/Images/Weapons/grds4.png"

#define LONG_BEAM "long_beam"
#define LONG_BEAM_PATH "/Images/Weapons/grdl4.png"

class ResourcePool{
private:
    SDL2pp::Renderer &renderer;
    std::map<std::string, std::unique_ptr<SDL2pp::Texture>> textures;
public:

    ResourcePool(SDL2pp::Renderer &rend) : renderer(rend) {} 

    void add_texture(std::string texture_name, std::string image_path) {
        textures[texture_name] = std::make_unique<SDL2pp::Texture>(
        renderer,
        SDL2pp::Surface(RESOURCES_PATH + image_path).SetColorKey(true, 0)
    );
    }

    void add_short_beam() {
        add_texture(SHORT_BEAM, SHORT_BEAM_PATH);
    }

    SDL2pp::Texture* get_short_beam_texture() {
        return textures[SHORT_BEAM].get();
    }

    void add_long_beam() {
        add_texture(LONG_BEAM, LONG_BEAM_PATH);
    }

    SDL2pp::Texture* get_long_beam_texture() {
        return textures[LONG_BEAM].get();
    }
};

#endif
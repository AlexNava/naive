#include "textureloader.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

TextureLoader &TextureLoader::getInstance()
{
    static TextureLoader singleton;
    return singleton;
}

TextureLoader::TextureLoader()
{
    SDL_Surface *imgSurf = IMG_Load("test");
}

TextureLoader::~TextureLoader()
{

}

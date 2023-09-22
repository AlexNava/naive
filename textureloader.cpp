#include "SDL2/SDL.h"
#include "SDL_image.h"

#include "texture.hpp"
#include "textureloader.hpp"
#include "palette.hpp"

TextureLoader &TextureLoader::getInstance()
{
    static TextureLoader singleton;
    return singleton;
}

void TextureLoader::loadPaletteFromFile(std::string filePath)
{
    SDL_RWops *rwop;
    rwop = SDL_RWFromFile(filePath.c_str(), "rb");
    if (IMG_isGIF(rwop) || IMG_isPCX(rwop))
    {
        SDL_Surface *imgSurf = IMG_Load(filePath.c_str());
        if (imgSurf != nullptr
            && imgSurf->format != nullptr
            && imgSurf->format->palette != nullptr)
        {
            for (uint16_t col = 0; col < imgSurf->format->palette->ncolors && col < constants::PALETTE_ENTRIES; ++col)
                Palette::getInstance().setColor(col,
                        imgSurf->format->palette->colors[col].r,
                        imgSurf->format->palette->colors[col].g,
                        imgSurf->format->palette->colors[col].b);
            SDL_FreeSurface(imgSurf);
        }
    }
}

Texture *TextureLoader::newTextureFromFile(std::string filePath)
{
    Texture *pResult = nullptr;
    SDL_RWops *rwop;
    rwop = SDL_RWFromFile(filePath.c_str(), "rb");
    if (IMG_isGIF(rwop) || IMG_isPCX(rwop))
    {
        SDL_Surface *imgSurf = IMG_Load(filePath.c_str());
        if (imgSurf != nullptr && imgSurf->format->BitsPerPixel == 8)
        {
            pResult = new Texture(imgSurf->w, imgSurf->h, TextureType::COLOR);

            pResult->setTexels((col_t*)imgSurf->pixels, imgSurf->w * imgSurf->h, 0);
        }
    }
    return pResult;
}

TextureLoader::TextureLoader()
{
}

TextureLoader::~TextureLoader()
{

}

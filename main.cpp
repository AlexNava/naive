#include <cstdio>

#include "SDL2/SDL.h"

#include "video.hpp"
#include "palette.hpp"
#include "textureloader.hpp"

int main(int argc, char **argv)
{
    TextureLoader::getInstance().loadPaletteFromFile("data/awesome-lion.gif");

    uint32_t time = SDL_GetTicks();
    Palette::getInstance().computeLookupTables();
    time = SDL_GetTicks() - time;
    printf ("lookup tables computed in %d ms\n", time);

    Texture *pTex = TextureLoader::getInstance().newTextureFromFile("data/awesome-lion-512.gif");

    Video videoMgr;
    videoMgr.setFullscreen(true);
    videoMgr.present();
    SDL_Delay(2000);
    videoMgr.setInternalResolution(640, 400);
    videoMgr.present();
    SDL_Delay(2000);
    videoMgr.setWindowedScale(1);
    videoMgr.setFullscreen(false);
    videoMgr.present();
    SDL_Delay(2000);
    videoMgr.setInternalResolution(320, 200);
    videoMgr.setWindowedScale(3);
    videoMgr.present();
    SDL_Delay(2000);
    return 0;
}

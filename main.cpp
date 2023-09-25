#include <cstdio>

#include "SDL2/SDL.h"

#include "video.hpp"
#include "palette.hpp"
#include "texture.hpp"
#include "textureloader.hpp"

void testBlit(Video &video, Texture &texture, int mipLevel);

int main(int argc, char **argv)
{
    TextureLoader::getInstance().loadPaletteFromFile("data/awesome-lion-1024.pcx");

    uint32_t time = SDL_GetTicks();
    Palette::getInstance().computeLookupTables();
    time = SDL_GetTicks() - time;
    printf ("lookup tables computed in %d ms\n", time);

    Texture *pTex = TextureLoader::getInstance().newTextureFromFile("data/awesome-lion-1024.pcx");

    Video videoMgr;
    videoMgr.setWindowedScale(1);
    videoMgr.setInternalResolution(1024, 1024);
    SDL_Delay(1000);
    int mip = 0;
    int mipDir = 1;
    int cnt = 0;
    while(true)
    {
        testBlit(videoMgr, *pTex, mip);
        videoMgr.present();
        mip += mipDir;
        if (mip <= 0 || mip >= pTex->mipNumber())
            mipDir = -mipDir;
        SDL_Delay(200);
        ++cnt;
        if (cnt == 25)
            videoMgr.setFullscreen(true);
        if (cnt == 50)
            videoMgr.setFullscreen(false);
        /*if (cnt == 75)
        {
            videoMgr.setInternalResolution(640, 480);
            videoMgr.setWindowedScale(2);
        }*/
    }

    delete pTex;

    return 0;
}

void testBlit(Video &video, Texture &texture, int mipLevel)
{
    int w = video.screenWidth();
    col_t *buf = video.getVgaScreen();

    for (int y = 0; y < std::min(1024, video.screenHeight()); ++y)
        for (int x = 0; x < std::min(1024, video.screenWidth()); ++x)
        {
            *functions::accessArray(buf, x, y, w) = texture.getTexel(x, y, mipLevel);
        }
}

#include <cstdio>

#include "SDL2/SDL.h"

#include "video.hpp"
#include "screen.hpp"
#include "palette.hpp"
#include "texture.hpp"
#include "textureloader.hpp"
#include "3d/mesh.hpp"
#include "3d/objloader.hpp"

void testBlit(Video &video, Texture &texture, int mipLevel);

int main(int argc, char **argv)
{
    Mesh *pA112 = ObjLoader::getInstance().loadMesh("data/a112_cleaned.obj");

    TextureLoader::getInstance().loadPaletteFromFile("data/awesome-lion-1024.pcx");

    uint32_t time = SDL_GetTicks();
    Palette::getInstance().computeLookupTables();
    time = SDL_GetTicks() - time;
    printf ("lookup tables computed in %d ms\n", time);

    Texture *pTex = TextureLoader::getInstance().newTextureFromFile("data/awesome-lion-1024.pcx");

    Video videoMgr;
    videoMgr.setWindowedScale(1);
    videoMgr.setInternalResolution(640, 480);
    SDL_Delay(1000);
    int mip = 0;
    int mipDir = 1;
    int cnt = 0;
    time = SDL_GetTicks();
    while(true)
    {
        for (int i = 0; i < 1; ++i)
            testBlit(videoMgr, *pTex, mip);
        videoMgr.present();
        mip += mipDir;
        if (mip <= 0 || mip >= pTex->mipNumber())
            mipDir = -mipDir;
        //SDL_Delay(200);
        ++cnt;
        //if (cnt == 25)
        //    videoMgr.setFullscreen(true);
        //if (cnt == 50)
        //    videoMgr.setFullscreen(false);
        /*if (cnt == 75)
        {
            videoMgr.setInternalResolution(640, 480);
            videoMgr.setWindowedScale(2);
        }*/
        if (cnt == 1000)
        {
            time = SDL_GetTicks() - time;
            printf ("1000 frames in %d ms = %f fps\n", time, (1000000.0 / time));
            cnt = 0;
            time = SDL_GetTicks();
        }
    }

    delete pTex;

    return 0;
}

void testBlit(Video &video, Texture &texture, int mipLevel)
{
    int w = video.screenWidth();
    Screen *pScr = video.pVgaScreen();
    col_t *buf = pScr->pBuffer();

    texture.setMipLevel(mipLevel);
    for (int y = 0; y < std::min(1024, video.screenHeight()); ++y)
        for (int x = 0; x < std::min(1024, video.screenWidth()); ++x)
        {
            *functions::accessArray(buf, x, y, w) = texture.getTexel(x, y);
            //pScr->putPixel(x, y, texture.getTexel(x, y));
        }
}

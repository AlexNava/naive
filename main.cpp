#include <cstdio>
#include <cmath>
#include "naive.hpp"
/*
#include "SDL2/SDL.h"

#include "video.hpp"
#include "screen.hpp"
#include "palette.hpp"
#include "texture.hpp"
*/
#include "3d/mesh.hpp"
#include "3d/objloader.hpp"
#include "textureloader.hpp"

void testBlit(Video &video, Texture &texture, int mipLevel);

int main(int argc, char **argv)
{
    const int scrW = 640;
    const int scrH = 480;

    Mesh *pA112 = ObjLoader::getInstance().loadMesh("data/a112_cleaned.obj");

    TextureLoader::getInstance().loadPaletteFromFile("data/awesome-lion-1024.pcx");

    uint32_t time = SDL_GetTicks();
    uint32_t currTime;
    Palette::getInstance().computeLookupTables();
    time = SDL_GetTicks() - time;
    printf ("lookup tables computed in %d ms\n", time);

    Texture *pTex = TextureLoader::getInstance().newTextureFromFile("data/awesome-lion-1024.pcx");

    Video videoMgr;
    videoMgr.setWindowedScale(1);
    videoMgr.setInternalResolution(scrW, scrH);
    //videoMgr.setFullscreen(true);
    //SDL_Delay(1000);
    int mip = 0;
    int mipDir = 1;

    Rasterizer *pRasterizer = new Rasterizer();
    pRasterizer->setPTargetScreen(videoMgr.pVgaScreen());
    pRasterizer->setPTexture(pTex);

    RasterVertex a, b, c;

    int cnt = 0;
    time = SDL_GetTicks();
    while(!Events::getInstance().isQuitRequested())
    {
        Events::getInstance().poll(); 
        currTime = SDL_GetTicks();

        //testBlit(videoMgr, *pTex, mip);
        //testBlit(videoMgr, *pTex, 0);
        screenOps::clearScreen(videoMgr.pVgaScreen(), 6);

        a.u = constants::TEXTURE_SPACE_SIZE * 0.5;
        a.v = 0;
        b.u = constants::TEXTURE_SPACE_SIZE - 1;
        b.v = constants::TEXTURE_SPACE_SIZE * 0.867;
        c.u = 0;
        c.v = constants::TEXTURE_SPACE_SIZE * 0.867;

        int idx = 1;
        for (int y = -200; y <= 200; y += 50)
        {
            for (int x = -250; x <= 250; x += 50)
            {
                a.x = x + scrW / 2 + 50 * cos((currTime + x) * M_PI / 2000);
                a.y = y + scrH / 2 + 50 * sin((currTime + x) * M_PI / 2000);
                b.x = x + scrW / 2 + 50 * cos((currTime + x) * M_PI / 2000 + M_PI * 2.0 / 3.0);
                b.y = y + scrH / 2 + 50 * sin((currTime + x) * M_PI / 2000 + M_PI * 2.0 / 3.0);
                c.x = x + scrW / 2 + 50 * cos((currTime + x) * M_PI / 2000 + M_PI * 4.0 / 3.0);
                c.y = y + scrH / 2 + 50 * sin((currTime + x) * M_PI / 2000 + M_PI * 4.0 / 3.0);
                pTex->setMipLevel((x + y + 450) / 90);
                pRasterizer->renderTriangle(&a, &b, &c, (col_t)idx++, materialFlags::TEXTURED);
            }
        }

        a.x = scrW / 2 + 280 * cos((currTime) * M_PI / 2000);
        a.y = scrH / 2 + 280 * sin((currTime) * M_PI / 2000);
        a.light = 127 + 128 * sin((currTime) * M_PI / 2000);
        b.x = scrW / 2 + 280 * cos((currTime) * M_PI / 2000 + M_PI * 2.0 / 3.0);
        b.y = scrH / 2 + 280 * sin((currTime) * M_PI / 2000 + M_PI * 2.0 / 3.0);
        b.light = 127;
        c.x = scrW / 2 + 280 * cos((currTime) * M_PI / 2000 + M_PI * 4.0 / 3.0);
        c.y = scrH / 2 + 280 * sin((currTime) * M_PI / 2000 + M_PI * 4.0 / 3.0);
        c.light = 255;
        pTex->setMipLevel(0);
        pRasterizer->renderTriangle(&a, &b, &c, (col_t)128, materialFlags::TEXTURED | materialFlags::ADD_TRANSPARENT | materialFlags::SOFT_SHADED);

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


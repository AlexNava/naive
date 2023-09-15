#include <SDL2/SDL.h>
#include <cstdio>

#include "video.hpp"
#include "palette.hpp"

int main(int argc, char **argv)
{
    Palette palette;

    uint32_t time = SDL_GetTicks();
    palette.computeLookupTables();
    time = SDL_GetTicks() - time;
    printf ("lookup tables computed in %d ms\n", time);

    Video videoMgr;
    videoMgr.setFullscreen(true);
    SDL_Delay(2000);
    videoMgr.setInternalResolution(640, 400);
    SDL_Delay(2000);
    videoMgr.setWindowedScale(1);
    videoMgr.setFullscreen(false);
    SDL_Delay(2000);
    videoMgr.setInternalResolution(320, 200);
    videoMgr.setWindowedScale(3);
    SDL_Delay(2000);
    return 0;
}

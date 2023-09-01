#include "renderer.hpp"
#include "SDL.h"

int main(int argc, char **argv)
{
    Renderer rend;

    rend.setFullscreen(true);
    SDL_Delay(2000);
    rend.setInternalResolution(640, 400);
    SDL_Delay(2000);
    rend.setWindowedScale(1);
    rend.setFullscreen(false);
    SDL_Delay(2000);
    rend.setInternalResolution(320, 200);
    rend.setWindowedScale(3);
    SDL_Delay(2000);
    return 0;
}

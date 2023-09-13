#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "SDL2/SDL.h"

class Renderer
{
public:
    Renderer();
    bool setInternalResolution(int width, int height);
    bool setFullscreen(bool fullscreen);
    bool setWindowedScale(int pixelScale);

private:
    glm::dvec3 points[100];

    int m_screenWidth;
    int m_screenHeight;
    int m_windowScale;
    bool m_fullscreen;

    char *vgaScreen;

    char *virScr;
    char *virScr2;
    char rr[256], gg[256], bb[256];
    char rd, gr, bl;

    SDL_Window   *m_pWin         = nullptr;
    SDL_Renderer *m_pSdlRenderer = nullptr;
    SDL_Texture  *m_pSdlTexture  = nullptr;
    SDL_Surface  *m_pRgbSurface  = nullptr;
    SDL_Surface  *m_pVgaSurface  = nullptr;

    SDL_Color sdlPalette[256];
};

#endif // RENDERER_HPP

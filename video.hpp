#ifndef VIDEO_HPP
#define VIDEO_HPP

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "SDL2/SDL.h"

#include "naive_defs.hpp"

class Screen;

class Video
{
public:
    Video();
    bool setInternalResolution(int width, int height);
    bool setFullscreen(bool fullscreen);
    bool setWindowedScale(int pixelScale);

    inline int screenHeight() const
    {
        return m_screenHeight;
    }

    inline int screenWidth() const
    {
        return m_screenWidth;
    }

    void present();

    Screen *pVgaScreen() const;

private:
    glm::dvec3 points[100];

    int m_screenWidth;
    int m_screenHeight;
    int m_windowScale;
    bool m_fullscreen;

    col_t *m_pVgaBuffer;
    Screen *m_pVgaScreen;

    SDL_Window   *m_pWin         = nullptr;
    SDL_Renderer *m_pSdlRenderer = nullptr;
    SDL_Texture  *m_pSdlTexture  = nullptr;
    SDL_Surface  *m_pRgbSurface  = nullptr;
    SDL_Surface  *m_pVgaSurface  = nullptr;
};

#endif // VIDEO_HPP

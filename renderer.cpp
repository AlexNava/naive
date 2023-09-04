#include "renderer.hpp"

Renderer::Renderer()
{
    m_screenWidth  = 320; // a true classic
    m_screenHeight = 200;
    m_windowScale   = 2;
    m_fullscreen   = false;

    win = SDL_CreateWindow("naive", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth * m_windowScale, m_screenHeight * m_windowScale, 0);
    sdlRenderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);

    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(sdlRenderer, m_screenWidth, m_screenHeight);

    sdlTexture = SDL_CreateTexture(
        sdlRenderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        m_screenWidth, m_screenHeight);
    rgbSurface = SDL_CreateRGBSurface(0, m_screenWidth, m_screenHeight, 32, 0, 0, 0, 0);
    vgaSurface = SDL_CreateRGBSurface(0, m_screenWidth, m_screenHeight, 8, 0, 0, 0, 0);
}

bool Renderer::setInternalResolution(int width, int height)
{
    if (width > 0 && height > 0)
    {
        bool result = true;
        int ret = 0;

        m_screenWidth = width;
        m_screenHeight = height;

        ret = SDL_RenderSetLogicalSize(sdlRenderer, m_screenWidth, m_screenHeight);
        result &= (ret == 0);

        SDL_DestroyTexture(sdlTexture);
        SDL_FreeSurface(rgbSurface);
        SDL_FreeSurface(vgaSurface);

        sdlTexture = SDL_CreateTexture(
            sdlRenderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            m_screenWidth, m_screenHeight);
        rgbSurface = SDL_CreateRGBSurface(0, m_screenWidth, m_screenHeight, 32, 0, 0, 0, 0);
        vgaSurface = SDL_CreateRGBSurface(0, m_screenWidth, m_screenHeight, 8, 0, 0, 0, 0);

        result &= (sdlTexture != nullptr);
        result &= (rgbSurface != nullptr);
        result &= (vgaSurface != nullptr);

        result &= setWindowedScale(m_windowScale);
        return result;
    }

    return false;
}

bool Renderer::setFullscreen(bool fullscreen)
{
    return (SDL_SetWindowFullscreen(win, (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0)) == 0);
}

bool Renderer::setWindowedScale(int pixelScale)
{
    if (pixelScale > 0)
    {
        m_windowScale = pixelScale;
        SDL_SetWindowSize(win, m_screenWidth * m_windowScale, m_screenHeight * m_windowScale);
        return true;
    }
    return false;
}

char *Renderer::getVgaScreen() const
{
    return vgaScreen;
}

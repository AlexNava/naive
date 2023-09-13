#include "renderer.hpp"

Renderer::Renderer()
{
    m_screenWidth  = 320; // a true classic
    m_screenHeight = 200;
    m_windowScale  = 2;
    m_fullscreen   = false;

    m_pWin = SDL_CreateWindow("naive", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth * m_windowScale, m_screenHeight * m_windowScale, 0);
    m_pSdlRenderer = SDL_CreateRenderer(m_pWin, -1, SDL_RENDERER_PRESENTVSYNC);

    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(m_pSdlRenderer, m_screenWidth, m_screenHeight);

    m_pSdlTexture = SDL_CreateTexture(
        m_pSdlRenderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        m_screenWidth, m_screenHeight);
    m_pRgbSurface = SDL_CreateRGBSurface(0, m_screenWidth, m_screenHeight, 32, 0, 0, 0, 0);
    m_pVgaSurface = SDL_CreateRGBSurface(0, m_screenWidth, m_screenHeight, 8, 0, 0, 0, 0);
}

bool Renderer::setInternalResolution(int width, int height)
{
    if (width > 0 && height > 0)
    {
        bool result = true;
        int ret = 0;

        m_screenWidth = width;
        m_screenHeight = height;

        ret = SDL_RenderSetLogicalSize(m_pSdlRenderer, m_screenWidth, m_screenHeight);
        result &= (ret == 0);

        SDL_DestroyTexture(m_pSdlTexture);
        SDL_FreeSurface(m_pRgbSurface);
        SDL_FreeSurface(m_pVgaSurface);

        m_pSdlTexture = SDL_CreateTexture(
            m_pSdlRenderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            m_screenWidth, m_screenHeight);
        m_pRgbSurface = SDL_CreateRGBSurface(0, m_screenWidth, m_screenHeight, 32, 0, 0, 0, 0);
        m_pVgaSurface = SDL_CreateRGBSurface(0, m_screenWidth, m_screenHeight, 8, 0, 0, 0, 0);

        result &= (m_pSdlTexture != nullptr);
        result &= (m_pRgbSurface != nullptr);
        result &= (m_pVgaSurface != nullptr);

        result &= setWindowedScale(m_windowScale);
        return result;
    }

    return false;
}

bool Renderer::setFullscreen(bool fullscreen)
{
    return (SDL_SetWindowFullscreen(m_pWin, (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0)) == 0);
}

bool Renderer::setWindowedScale(int pixelScale)
{
    if (pixelScale > 0)
    {
        m_windowScale = pixelScale;
        SDL_SetWindowSize(m_pWin, m_screenWidth * m_windowScale, m_screenHeight * m_windowScale);
        return true;
    }
    return false;
}

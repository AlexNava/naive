#include "renderer.hpp"

Renderer::Renderer()
{
    m_screenWidth  = 320; // a true classic
    m_screenHeight = 200;
    m_windowScale   = 2;
    m_fullscreen   = false;

    win = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth * m_windowScale, m_screenHeight * m_windowScale, 0);
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
    m_screenWidth = width;
    m_screenHeight = height;

    SDL_RenderSetLogicalSize(sdlRenderer, m_screenWidth, m_screenHeight);

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

}

bool Renderer::setFullscreen(bool fullscreen)
{
    SDL_SetWindowFullscreen(win, (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0));
}

bool Renderer::setWindowedScale(int pixelScale)
{
    if (pixelScale > 0)
    {
        m_windowScale = pixelScale;
        SDL_SetWindowSize(win, m_screenWidth * m_windowScale, m_screenHeight * m_windowScale);
    }
}

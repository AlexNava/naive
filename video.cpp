#include "video.hpp"
#include "palette.hpp"
#include "screen.hpp"

Video::Video()
{
    m_screenWidth  = 320; // a true classic
    m_screenHeight = 200;
    m_windowScale  = 2;
    m_fullscreen   = false;

    m_pWin = SDL_CreateWindow("naive", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth * m_windowScale, m_screenHeight * m_windowScale, 0);
    m_pSdlRenderer = SDL_CreateRenderer(m_pWin, -1, 0);

    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(m_pSdlRenderer, m_screenWidth, m_screenHeight);

    m_pSdlTexture = SDL_CreateTexture(
        m_pSdlRenderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        m_screenWidth, m_screenHeight);
    m_pRgbSurface = SDL_CreateRGBSurface(0, m_screenWidth, m_screenHeight, 32, 0, 0, 0, 0);
    m_pVgaSurface = SDL_CreateRGBSurface(0, m_screenWidth, m_screenHeight, 8 * sizeof(col_t), 0, 0, 0, 0);
    m_pVgaBuffer = (col_t*)m_pVgaSurface->pixels;
    m_pVgaScreen = new Screen(m_screenWidth, m_screenHeight, m_pVgaBuffer);

    Palette::getInstance().setSystemPalette(m_pVgaSurface->format->palette);
}

bool Video::setInternalResolution(int width, int height)
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
        m_pVgaSurface = SDL_CreateRGBSurface(0, m_screenWidth, m_screenHeight, 8 * sizeof(col_t), 0, 0, 0, 0);
        m_pVgaBuffer = (col_t*)m_pVgaSurface->pixels;

        if (m_pVgaScreen)
            delete m_pVgaScreen;
        m_pVgaScreen = new Screen(m_screenWidth, m_screenHeight, m_pVgaBuffer);

        Palette::getInstance().setSystemPalette(m_pVgaSurface->format->palette);

        result &= (m_pSdlTexture != nullptr);
        result &= (m_pRgbSurface != nullptr);
        result &= (m_pVgaSurface != nullptr);

        result &= setWindowedScale(m_windowScale);
        return result;
    }

    return false;
}

bool Video::setFullscreen(bool fullscreen)
{
    return (SDL_SetWindowFullscreen(m_pWin, (fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0)) == 0);
}

bool Video::setWindowedScale(int pixelScale)
{
    if (pixelScale > 0)
    {
        m_windowScale = pixelScale;
        SDL_SetWindowSize(m_pWin, m_screenWidth * m_windowScale, m_screenHeight * m_windowScale);
        return true;
    }
    return false;
}

void Video::present()
{
    void *texturePixels;
    int texturePitch;

    if (!m_pVgaSurface)
        return;


    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        if (evt.type == SDL_QUIT)
        {
            exit(0);
        }
    }

    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);


    SDL_ClearError();
    SDL_BlitSurface(m_pVgaSurface, NULL, m_pRgbSurface, NULL); // with format conversion

    SDL_LockTexture(m_pSdlTexture, NULL, &texturePixels, &texturePitch);
        SDL_ConvertPixels(m_pRgbSurface->w, m_pRgbSurface->h,
        m_pRgbSurface->format->format,
        m_pRgbSurface->pixels, m_pRgbSurface->pitch,
        SDL_PIXELFORMAT_ARGB8888,
        texturePixels, texturePitch);
    SDL_UnlockTexture(m_pSdlTexture);

    SDL_RenderClear(m_pSdlRenderer);
    SDL_RenderCopy(m_pSdlRenderer, m_pSdlTexture, NULL, NULL);
    SDL_RenderPresent(m_pSdlRenderer);
}

Screen *Video::pVgaScreen() const
{
    return m_pVgaScreen;
}
/*
int Video::screenHeight() const
{
    return m_screenHeight;
}

int Video::screenWidth() const
{
    return m_screenWidth;
}
*/

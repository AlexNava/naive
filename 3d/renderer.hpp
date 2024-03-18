#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "naive_defs.hpp"

class Screen;
class Obj3D;

class Renderer
{
public:
    Renderer(Screen *targetScreen);

private:
    int m_screenWidth;
    int m_screenHeight;
    Screen *m_pScreen;
};

#endif // RENDERER_HPP

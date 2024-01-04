#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "naive_defs.hpp"
#include "glm/glm.hpp"

class Obj3D;

class Renderer
{
public:
    Renderer();

private:
    int m_screenWidth;
    int m_screenHeight;
    col_t *m_pScreeen;
};

#endif // RENDERER_HPP

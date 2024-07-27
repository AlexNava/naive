#include "viewport.hpp"


Viewport::Viewport()
{
    setLimits(0, 0, 0, 0);
}

Viewport::Viewport(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    setLimits(x, y, w, h);
}

void Viewport::setLimits(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    m_x = x;
    m_y = y;
    m_h = h;
    m_w = w;
}

uint16_t Viewport::x() const
{
    return m_x;
}

uint16_t Viewport::y() const
{
    return m_y;
}

uint16_t Viewport::w() const
{
    return m_w;
}

uint16_t Viewport::h() const
{
    return m_h;
}

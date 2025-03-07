#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <cstdint>

class Viewport
{
public:
    Viewport();
    Viewport(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void setLimits(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

    uint16_t x() const;

    uint16_t y() const;

    uint16_t w() const;

    uint16_t h() const;

private:
    uint16_t m_x;
    uint16_t m_y;
    uint16_t m_w;
    uint16_t m_h;
};

#endif // VIEWPORT_HPP

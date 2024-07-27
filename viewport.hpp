#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <cstdint>

class Viewport
{
public:
    Viewport();
private:
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
};

#endif // VIEWPORT_HPP

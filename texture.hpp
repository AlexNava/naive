#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstdint>

class Texture
{
public:
    Texture();
private:
    uint16_t m_width;
    uint16_t m_height;
    char *m_pixels;
};

#endif // TEXTURE_HPP

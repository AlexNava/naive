#include "texture.hpp"

Texture::Texture(uint16_t w, uint16_t h, TextureType texType, bool calcMips)
{
    m_width = w;
    m_height = h;
    m_type = texType;
    m_calcMips = calcMips;

    m_mipNumber = 0;
    // add until w or h are divisible by 2
}

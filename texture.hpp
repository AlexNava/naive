#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstdint>
#include "naive_defs.hpp"

enum class TextureType : uint8_t
{
    COLOR,
    COLOR_AND_ALPHA,
    LIGHT,
    UNDEFINED
};

class Texture
{
public:
    Texture(uint16_t w, uint16_t h, TextureType texType, bool calcMips = false);

private:
    uint16_t m_width;
    uint16_t m_height;
    char **m_texels;
    uint8_t m_mipNumber;
    TextureType m_type;
    bool m_calcMips;
};

#endif // TEXTURE_HPP

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstdint>
#include <vector>

#include "naive_defs.hpp"

class Palette;

enum class TextureType : uint8_t
{
    COLOR,
    COLOR_AND_KEY,
    COLOR_AND_ALPHA, // will i ever use this?
    LIGHT,
    UNDEFINED
};

class Texture
{
public:
    Texture(uint16_t w, uint16_t h, TextureType texType);
    ~Texture();

    col_t getTexel(uint16_t u, uint16_t v, uint8_t mipLevel) const;

private:
    bool m_isValid;
    uint16_t m_width;
    uint16_t m_height;
    std::vector<col_t*> m_colorTexels;
    std::vector<col_t*> m_alphaTexels;
    uint8_t m_mipNumber;
    TextureType m_type;
    bool m_calcMips;
    Palette *m_palette;
    col_t m_keyColor; // transparency

    uint8_t uShift;
    uint8_t vShift;

    void calculateMips();
    void deleteBitmaps();
    col_t blendColors(col_t a, col_t b, col_t c, col_t d) const; // used to calculate mip levels
    col_t *accessArray(col_t *buf, uint16_t x, uint16_t y, uint16_t rowLength) const;
};

#endif // TEXTURE_HPP

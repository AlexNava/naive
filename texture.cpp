#include "texture.hpp"
#include "palette.hpp"
#include <cstring>

Texture::Texture(uint16_t w, uint16_t h, TextureType texType)
{
    m_isValid = false;
    m_width = 0;
    m_height = 0;
    m_uvMask = constants::TEXTURE_SPACE_SIZE - 1;

    if ((functions::isPowOf2(w) && functions::isPowOf2(h))
        && (w <= constants::TEXTURE_SPACE_SIZE) && (h <= constants::TEXTURE_SPACE_SIZE)
        && (texType == TextureType::COLOR || texType == TextureType::COLOR_AND_ALPHA || texType == TextureType::LIGHT))
    {
        m_width = w;
        m_height = h;
        m_type = texType;
        m_isValid = true;
        uShift = functions::getPowOf2(constants::TEXTURE_SPACE_SIZE / w);
        vShift = functions::getPowOf2(constants::TEXTURE_SPACE_SIZE / h);
        m_mipNumber = 1;
        m_calcMips = true;
        m_palette = &Palette::getInstance();
        col_t *pColorBuf = new col_t[w * h];
        m_colorTexels.push_back(pColorBuf);
        col_t *pAlphaBuf = new col_t[w * h];
        m_alphaTexels.push_back(pAlphaBuf);
    }

    setMipLevel(0);
}

Texture::~Texture()
{
    deleteBitmaps();
}
/*
col_t Texture::getTexel(uint16_t u, uint16_t v) const
{
    u >>= m_effUShift;
    v >>= m_effVShift;

    return *(functions::accessArray(m_colorTexels[m_mipLevel], u, v, m_effWidth));
}
*/
void Texture::calculateMips()
{
    uint16_t h = m_height;
    uint16_t w = m_width;
    deleteBitmaps(1);
    m_mipNumber = 1;

    // add mips until w or h are divisible by 2
    while (!(h & 1) && !(w & 1))
    {
        h >>= 1;
        w >>= 1;

        col_t *hiBuf = m_colorTexels[m_mipNumber - 1];
        col_t *buffer = new col_t [h * w];
        col_t *alphaBuf = new col_t [h * w];

        for (uint16_t u = 0; u < w; ++u)
            for (uint16_t v = 0; v < h; ++v)
            {
                col_t *pNewTexel = functions::accessArray(buffer, u, v, w);
                *pNewTexel = blendColors(
                        *functions::accessArray(hiBuf, u * 2, v * 2, w * 2),
                        *functions::accessArray(hiBuf, u * 2 + 1, v * 2, w * 2),
                        *functions::accessArray(hiBuf, u * 2, v * 2 + 1, w * 2),
                        *functions::accessArray(hiBuf, u * 2 + 1, v * 2 + 1, w * 2)
                    );
            }

        m_colorTexels.push_back(buffer);
        m_alphaTexels.push_back(alphaBuf);
        ++m_mipNumber;
    }
}

void Texture::deleteBitmaps(uint8_t startingMipLevel)
{
    for (uint16_t iMip = startingMipLevel; iMip < m_colorTexels.size(); ++iMip)
    {
        delete[] m_colorTexels[iMip];
        m_colorTexels[iMip] = nullptr;
    }
    m_colorTexels.resize(startingMipLevel);

    for (uint16_t iMip = startingMipLevel; iMip < m_alphaTexels.size(); ++iMip)
    {
        delete[] m_alphaTexels[iMip];
        m_alphaTexels[iMip] = nullptr;
    }
    m_alphaTexels.resize(startingMipLevel);
}

col_t Texture::blendColors(col_t a, col_t b, col_t c, col_t d) const
{
    col_t colAB;
    col_t colCD;
    uint16_t average;

    switch (m_type) {
    case TextureType::COLOR:
        colAB = m_palette->getBlendedColor(a, b, constants::ALPHA_LEVELS / 2);
        colCD = m_palette->getBlendedColor(c, d, constants::ALPHA_LEVELS / 2);
        return m_palette->getBlendedColor(colAB, colCD, constants::ALPHA_LEVELS / 2);
        break;
    case TextureType::LIGHT:
        average = a + b + c + d;
        return average >> 2;
        break;
    default:
        return a; // todo other cases, count key pixels etc
    }
}

void Texture::setTexels(col_t *buffer, uint32_t size, uint8_t mipLevel)
{
    if ((mipLevel < m_mipNumber)
        && (size == uint32_t(m_width >> mipLevel) * uint32_t(m_height >> mipLevel)))
    {
        col_t *dest = m_colorTexels[mipLevel];
        memcpy(dest, buffer, size);

        if (m_calcMips && mipLevel == 0)
            calculateMips();
    }
}

void Texture::setCalcMips(bool newCalcMips)
{
    m_calcMips = newCalcMips;
}

uint8_t Texture::mipNumber() const
{
    return m_mipNumber;
}

void Texture::setMipLevel(uint8_t newMipLevel)
{
    m_mipLevel  = ((newMipLevel < m_mipNumber) ? newMipLevel : m_mipNumber - 1);
    m_effUShift = uShift + m_mipLevel;
    m_effVShift = vShift + m_mipLevel;
    m_effWidth  = m_width >> m_mipLevel;
}

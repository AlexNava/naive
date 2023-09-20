#include "texture.hpp"
#include "palette.hpp"

Texture::Texture(uint16_t w, uint16_t h, TextureType texType)
{
    m_isValid = false;

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
    }
}

Texture::~Texture()
{
    deleteBitmaps();
}

col_t Texture::getTexel(uint16_t u, uint16_t v, uint8_t mipLevel) const
{
    if (mipLevel >= m_mipNumber)
        mipLevel = m_mipNumber - 1;

    u >>= (uShift + mipLevel);
    v >>= (vShift + mipLevel);

    return *(accessArray(m_colorTexels[mipLevel], u, v, m_width >> mipLevel));
}

void Texture::calculateMips()
{
    m_mipNumber = 1;
    uint16_t h = m_height;
    uint16_t w = m_width;

    // add mips until w or h are divisible by 2
    while (!(h & 1) && !(w & 1))
    {
        h >>= 1;
        w >>= 1;
        ++m_mipNumber;

        col_t *hiBuf = m_colorTexels[m_mipNumber - 1];
        col_t *buffer = new col_t [h * w];

        for (uint16_t u = 0; u < w; ++u)
            for (uint16_t v = 0; v < h; ++v)
            {
                col_t *pNewTexel = accessArray(buffer, u, v, w);
                *pNewTexel = blendColors(
                        *accessArray(hiBuf, u * 2, v * 2, w * 2),
                        *accessArray(hiBuf, u * 2 + 1, v * 2, w * 2),
                        *accessArray(hiBuf, u * 2, v * 2 + 1, w * 2),
                        *accessArray(hiBuf, u * 2 + 1, v * 2 + 1, w * 2)
                    );
            }

        m_colorTexels.push_back(buffer);
    }
}

void Texture::deleteBitmaps()
{
    for (int iMip = 0; iMip < m_colorTexels.size(); ++iMip)
    {
        delete[] m_colorTexels[iMip];
        m_colorTexels[iMip] = nullptr;
    }
    m_colorTexels.clear();

    for (int iMip = 0; iMip < m_alphaTexels.size(); ++iMip)
    {
        delete[] m_alphaTexels[iMip];
        m_alphaTexels[iMip] = nullptr;
    }
    m_alphaTexels.clear();
}

col_t Texture::blendColors(col_t a, col_t b, col_t c, col_t d) const
{
    col_t colAB;
    col_t colCD;
    uint16_t average;

    switch (m_type) {
    case TextureType::COLOR:
        colAB = m_palette->getBlendedColor(a, b, constants::ALPHA_LEVELS / 2);
        colCD = m_palette->getBlendedColor(d, c, constants::ALPHA_LEVELS / 2);
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

col_t *Texture::accessArray(col_t *buf, uint16_t x, uint16_t y, uint16_t rowLength) const
{
    return buf + x + y * rowLength;
}

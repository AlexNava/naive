#include "palette.hpp"
#include "renderer.hpp"

Palette::Palette(Renderer *pRenderer)
{
    m_pRenderer = pRenderer;
}

void Palette::setColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b)
{
    m_palette[index].r = r;
    m_palette[index].g = g;
    m_palette[index].b = b;
}

void Palette::computeLookupTables()
{
    for (uint8_t color1 = 0; color1 < constants::PALETTE_ENTRIES; ++color1)
        for (uint8_t light = 0; light < constants::LIGHT_LEVELS; ++light)
        {
            SDL_Color targetColor = color1;
            targetColor.r *= light; targetColor.r /= (constants::LIGHT_LEVELS - 1);
            targetColor.g *= light; targetColor.g /= (constants::LIGHT_LEVELS - 1);
            targetColor.b *= light; targetColor.b /= (constants::LIGHT_LEVELS - 1);

            m_lightTable[color1][light] = computeNearestColor(targetColor);
        }

    for (uint8_t color1 = 0; color1 < constants::PALETTE_ENTRIES; ++color1)
        for (uint8_t color2 = 0; color2 < constants::PALETTE_ENTRIES; ++color2)
        {

        }
}

double Palette::computeDistance(SDL_Color color1, SDL_Color color2) const
{
    return std::sqrt(
            std::pow(((double)color1.r - (double)color2.r), 2.0)
            + std::pow(((double)color1.g - (double)color2.g), 2.0)
                + std::pow(((double)color1.b - (double)color2.b), 2.0));
}

uint8_t Palette::computeNearestColor(SDL_Color target)
{
    uint8_t minIndex = 0;
    double minDistance = computeDistance(target, m_palette[0]);
    for (uint8_t index = 1; index < constants::PALETTE_ENTRIES; ++index)
    {
        double currentDistance = computeDistance(target, m_palette[index]);
        if (currentDistance < minDistance)
        {
            minDistance = currentDistance;
            minIndex = index;
        }
    }

    return minIndex;
}

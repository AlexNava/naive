#include <cmath>
#include <string>
#include <format>
#include <ios>
#include <iostream>
#include <fstream>

#include "palette.hpp"
#include "utils/hash.hpp"

Palette &Palette::getInstance()
{
    static Palette singleton;
    return singleton;
}

Palette::Palette()
{
}

void Palette::setSystemPalette(SDL_Palette *newSystemPalette)
{
    m_systemPalette = newSystemPalette;

    if (m_systemPalette != nullptr)
        SDL_SetPaletteColors(m_systemPalette, m_palette, 0, constants::PALETTE_ENTRIES);
}

void Palette::setColor(col_t index, uint8_t r, uint8_t g, uint8_t b)
{
    m_palette[index].r = r;
    m_palette[index].g = g;
    m_palette[index].b = b;

    if (m_systemPalette != nullptr)
        SDL_SetPaletteColors(m_systemPalette, &m_palette[index], index, 1);
}

SDL_Color Palette::getColor(col_t index)
{
    if (index < constants::PALETTE_ENTRIES)
        return m_palette[index];
}

void Palette::computeLookupTables()
{
    std::string paletteHash = hashPaletteName();

    // if a saved palette lookup table exists load it, otherwise calculate it and save it later
    if (!loadLookupTables(paletteHash))
    {
        for (uint16_t fgCol = 0; fgCol < constants::PALETTE_ENTRIES; ++fgCol)
            for (uint16_t light = 0; light < constants::LIGHT_LEVELS; ++light)
            {
                uint16_t r = m_palette[fgCol].r * light / (constants::LIGHT_LEVELS - 1);
                uint16_t g = m_palette[fgCol].g * light / (constants::LIGHT_LEVELS - 1);
                uint16_t b = m_palette[fgCol].b * light / (constants::LIGHT_LEVELS - 1);

                SDL_Color targetColor;
                targetColor.r = (r <= 255 ? r : 255);
                targetColor.g = (g <= 255 ? g : 255);
                targetColor.b = (b <= 255 ? b : 255);
                m_lightTable[fgCol][light] = computeNearestColor(targetColor);
            }

        for (uint16_t fgCol = 0; fgCol < constants::PALETTE_ENTRIES; ++fgCol)
            for (uint16_t bgCol = 0; bgCol < constants::PALETTE_ENTRIES; ++bgCol)
                for (uint16_t alpha = 0; alpha < constants::ALPHA_LEVELS; ++alpha)
                {
                    if (alpha == 0)
                        m_blendTable[fgCol][bgCol][alpha] = bgCol;
                    else if (alpha == constants::ALPHA_LEVELS - 1)
                        m_blendTable[fgCol][bgCol][alpha] = fgCol;
                    else
                    {
                        uint16_t r = m_palette[fgCol].r * alpha;
                        r += m_palette[bgCol].r * (constants::ALPHA_LEVELS - 1 - alpha);
                        r /= (constants::ALPHA_LEVELS - 1);
                        uint16_t g = m_palette[fgCol].g * alpha;
                        g += m_palette[bgCol].g * (constants::ALPHA_LEVELS - 1 - alpha);
                        g /= (constants::ALPHA_LEVELS - 1);
                        uint16_t b = m_palette[fgCol].b * alpha;
                        b += m_palette[bgCol].b * (constants::ALPHA_LEVELS - 1 - alpha);
                        b /= (constants::ALPHA_LEVELS - 1);

                        SDL_Color targetColor;
                        targetColor.r = (r <= 255 ? r : 255);
                        targetColor.g = (g <= 255 ? g : 255);
                        targetColor.b = (b <= 255 ? b : 255);
                        m_blendTable[fgCol][bgCol][alpha] = computeNearestColor(targetColor);
                    }
                }

        for (uint16_t fgCol = 0; fgCol < constants::PALETTE_ENTRIES; ++fgCol)
            for (uint16_t bgCol = 0; bgCol < constants::PALETTE_ENTRIES; ++bgCol)
            {
                uint16_t r = m_palette[fgCol].r;
                r += m_palette[bgCol].r;
                uint16_t g = m_palette[fgCol].g;
                g += m_palette[bgCol].g;
                uint16_t b = m_palette[fgCol].b;
                b += m_palette[bgCol].b;

                SDL_Color targetColor;
                targetColor.r = (r <= 255 ? r : 255);
                targetColor.g = (g <= 255 ? g : 255);
                targetColor.b = (b <= 255 ? b : 255);
                m_addBlendTable[fgCol][bgCol] = computeNearestColor(targetColor);
            }

        saveLookupTables(paletteHash);
    }
}

double Palette::computeDistance(SDL_Color color1, SDL_Color color2) const
{
    //double lum1 = std::sqrt(std::pow((double)color1.r, 2.0) + std::pow((double)color1.g, 2.0) + std::pow((double)color1.b, 2.0));
    //double lum2 = std::sqrt(std::pow((double)color2.r, 2.0) + std::pow((double)color2.g, 2.0) + std::pow((double)color2.b, 2.0));

    return //std::sqrt(
        std::pow(((double)color1.r - (double)color2.r), 2.0) * 3
        + std::pow(((double)color1.g - (double)color2.g), 2.0) * 4
        + std::pow(((double)color1.b - (double)color2.b), 2.0) * 2;
        /*std::pow((lum1 - lum2), 2.0) * 2*///);
}

col_t Palette::computeNearestColor(SDL_Color target)
{
    uint8_t minIndex = 0;
    double minDistance = computeDistance(target, m_palette[0]);
    for (uint16_t index = 1; index < constants::PALETTE_ENTRIES; ++index)
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

std::string Palette::hashPaletteName()
{
    NaiveHash hash;

    hash.accumulate(constants::PALETTE_ENTRIES);
    hash.accumulate(constants::ALPHA_LEVELS);
    hash.accumulate(constants::LIGHT_LEVELS);

    for (int i = 0; i < constants::PALETTE_ENTRIES; ++i)
    {
        hash.accumulate(m_palette[i].r << 32 | m_palette[i].g << 16 | m_palette[i].b);
    }

    return std::format("palette_{:x}.txt", hash.get());
}

void Palette::saveLookupTables(std::string filename)
{
    std::ofstream file(filename, std::ios::out);
    if (!file.good())
        return;

    file << constants::PALETTE_ENTRIES << std::endl;
    file << constants::LIGHT_LEVELS << std::endl;
    file << constants::ALPHA_LEVELS << std::endl;

    for (uint16_t fgCol = 0; fgCol < constants::PALETTE_ENTRIES; ++fgCol)
    {
        for (uint16_t light = 0; light < constants::LIGHT_LEVELS; ++light)
        {
            file << (int) m_lightTable[fgCol][light] << " ";
        }
        file << std::endl;
    }

    for (uint16_t fgCol = 0; fgCol < constants::PALETTE_ENTRIES; ++fgCol)
    {
        for (uint16_t bgCol = 0; bgCol < constants::PALETTE_ENTRIES; ++bgCol)
        {
            for (uint16_t alpha = 0; alpha < constants::ALPHA_LEVELS; ++alpha)
            {
                file << (int) m_blendTable[fgCol][bgCol][alpha] << " ";
            }
            file << std::endl;
        }
        file << std::endl;
    }

    for (uint16_t fgCol = 0; fgCol < constants::PALETTE_ENTRIES; ++fgCol)
    {
        for (uint16_t bgCol = 0; bgCol < constants::PALETTE_ENTRIES; ++bgCol)
        {
            file << (int) m_addBlendTable[fgCol][bgCol] << " ";
        }
        file << std::endl;
    }

    file.close();
}

bool Palette::loadLookupTables(std::string filename)
{
    std::ifstream file(filename, std::ios::in);

    if (!file.is_open())
        goto error;

    int tempInt;

    file >> tempInt;
    if (tempInt != constants::PALETTE_ENTRIES)
        goto error;
    file >> tempInt;
    if (tempInt != constants::LIGHT_LEVELS)
        goto error;
    file >> tempInt;
    if (tempInt != constants::ALPHA_LEVELS)
        goto error;

    for (uint16_t fgCol = 0; fgCol < constants::PALETTE_ENTRIES; ++fgCol)
        for (uint16_t light = 0; light < constants::LIGHT_LEVELS; ++light)
        {
            file >> tempInt;
            m_lightTable[fgCol][light] = tempInt;
        }

    for (uint16_t fgCol = 0; fgCol < constants::PALETTE_ENTRIES; ++fgCol)
        for (uint16_t bgCol = 0; bgCol < constants::PALETTE_ENTRIES; ++bgCol)
            for (uint16_t alpha = 0; alpha < constants::ALPHA_LEVELS; ++alpha)
            {
                file >> tempInt;
                m_blendTable[fgCol][bgCol][alpha] = tempInt;
            }

    for (uint16_t fgCol = 0; fgCol < constants::PALETTE_ENTRIES; ++fgCol)
        for (uint16_t bgCol = 0; bgCol < constants::PALETTE_ENTRIES; ++bgCol)
        {
            file >> tempInt;
            m_addBlendTable[fgCol][bgCol] = tempInt;
        }

    file.close();
    return true;

error:
    file.close();
    return false;
}

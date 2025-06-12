#ifndef PALETTE_HPP
#define PALETTE_HPP

#include <string>
#include "SDL2/SDL_pixels.h"
#include "naive_defs.hpp"

class Palette
{
public:
    static Palette &getInstance();
    void setColor(col_t index, uint8_t r, uint8_t g, uint8_t b);
    SDL_Color getColor(col_t index);

    void computeLookupTables();

    inline col_t getLightedColor(col_t color, col_t light) const
    {
        if (light <= constants::LIGHT_LEVELS)
            return m_lightTable[color][light];
        else
            return m_lightTable[color][constants::LIGHT_LEVELS - 1];
    }

    inline col_t getBlendedColor(col_t fgColor, col_t bgColor, alpha_t alpha) const
    {
        if (alpha < constants::ALPHA_LEVELS)
            return m_blendTable[fgColor][bgColor][alpha];
        else
            return m_blendTable[fgColor][bgColor][constants::ALPHA_LEVELS - 1];
    }

    inline col_t getAddedColor(col_t fgColor, col_t bgColor) const
    {
        return m_addBlendTable[fgColor][bgColor];
    }

    void setSystemPalette(SDL_Palette *newSystemPalette);

private:
    Palette();
    SDL_Palette *m_systemPalette;
    SDL_Color m_palette[constants::PALETTE_ENTRIES];
    uint8_t m_lightTable[constants::PALETTE_ENTRIES][constants::LIGHT_LEVELS];
    uint8_t m_blendTable[constants::PALETTE_ENTRIES][constants::PALETTE_ENTRIES][constants::ALPHA_LEVELS];
    uint8_t m_addBlendTable[constants::PALETTE_ENTRIES][constants::PALETTE_ENTRIES];

    double computeDistance(SDL_Color color1, SDL_Color color2) const;
    col_t computeNearestColor(SDL_Color target);

    std::string hashPaletteName();
    void saveLookupTables(std::string filename);
    bool loadLookupTables(std::string filename);

};

#endif // PALETTE_HPP

#ifndef PALETTE_HPP
#define PALETTE_HPP

#include "SDL2/SDL_pixels.h"
#include "naive_defs.hpp"

class Palette
{
public:
    static Palette &getInstance();
    void setColor(col_t index, uint8_t r, uint8_t g, uint8_t b);
    SDL_Color getColor(col_t index);

    void computeLookupTables();

    col_t getLightedColor(col_t color, col_t light);
    col_t getBlendedColor(col_t fgColor, col_t bgColor, alpha_t alpha);
    col_t getAddedColor(col_t color, light_t light);

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
};

#endif // PALETTE_HPP

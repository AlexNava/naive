#ifndef PALETTE_HPP
#define PALETTE_HPP

#include <SDL2/SDL_pixels.h>
#include "naive_defs.hpp"

class Renderer;

class Palette
{
public:
    Palette(Renderer *pRenderer = nullptr);
    void setColor(uint8_t index, uint8_t r, uint8_t g, uint8_t b);

    void computeLookupTables();

private:
    SDL_Color *m_pGlobalPalette;
    SDL_Color m_palette[constants::PALETTE_ENTRIES];
    uint8_t m_blendTable[constants::PALETTE_ENTRIES][constants::PALETTE_ENTRIES][constants::ALPHA_LEVELS];
    uint8_t m_addBlendTable[constants::PALETTE_ENTRIES][constants::PALETTE_ENTRIES];
    uint8_t m_lightTable[constants::PALETTE_ENTRIES][constants::LIGHT_LEVELS];

    double computeDistance(SDL_Color color1, SDL_Color color2) const;
    uint8_t computeNearestColor(SDL_Color target);

    Renderer *m_pRenderer;
};

#endif // PALETTE_HPP

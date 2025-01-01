#ifndef NAIVE_DEFS_HPP
#define NAIVE_DEFS_HPP

#include <cstdint>

typedef uint8_t col_t;
typedef uint8_t light_t;
typedef uint8_t alpha_t;
typedef uint16_t matFlags_t;
typedef uint16_t rendFlags_t;
typedef int32_t rendCoord_t;
typedef float rendDepth_t;

namespace constants
{
    const uint32_t PALETTE_ENTRIES    = 256;
    const uint32_t ALPHA_LEVELS       = 16;
    const uint32_t LIGHT_LEVELS       = 64;
    const uint32_t TEXTURE_SPACE_SIZE = 1024; // only powers of two, please :)
    const uint32_t LIGHT_SPACE_SIZE   = 256; // only powers of two, please :)
}

namespace materialFlags
{
    // no flags set = draw plain unlit triangle

    const matFlags_t TEXTURED         = 1 << 0;
    const matFlags_t TRANSPARENT      = 1 << 1; // blend to bg
    const matFlags_t ADD_TRANSPARENT  = 1 << 2; // blend additively
    const matFlags_t GOURAUD          = 1 << 3;
    const matFlags_t REFLECTION       = 1 << 4; // use additive spheremap
    const matFlags_t METAL_REFLECTION = 1 << 4; // use multiplicative spheremap (not yet)
}

namespace sortFlags
{
    const rendFlags_t BACKGROUND       = 1 << 0; // always render first
    const rendFlags_t FOREGROUND       = 1 << 1; // always render last
    const rendFlags_t DO_NOT_SORT      = 1 << 2; // do not z sort object
}

namespace functions
{
    int8_t getPowOf2(uint64_t number);
    bool   isPowOf2(uint64_t number);

    inline col_t *accessArray(col_t *buf, uint16_t x, uint16_t y, uint16_t rowLength)
    {
        return buf + x + y * rowLength;
    }

    inline int32_t getDither(uint16_t x, uint16_t y)
    {
        return 8 * ((x ^ y) & 1);
    }
}

#endif // NAIVE_DEFS_HPP

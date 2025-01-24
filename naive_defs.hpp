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
    const uint32_t LIGHT_LEVELS       = 16;
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

    inline int16_t getDither(uint16_t x, uint16_t y)
    {
        // dither pattern:
        //
        //    | 00 01 10 11 x
        //  --+--------------
        //  00|  1 13  4 16
        //  01|  9  5 12  8
        //  10|  3 15  2 14
        //  11| 11  7 10  6
        //   y|

        static const int d[16] =
            { 0 - 8, 12 - 8,  3 - 8, 15 - 8,
              8 - 8,  4 - 8, 11 - 8,  7 - 8,
              2 - 8, 14 - 8,  1 - 8, 13 - 8,
             10 - 8,  6 - 8,  9 - 8,  5 - 8};

        return d[x & 0b11 | (y & 0b11) << 2];
    }
}

#endif // NAIVE_DEFS_HPP

#ifndef NAIVE_DEFS_HPP
#define NAIVE_DEFS_HPP

#include <cstdint>

typedef uint8_t col_t;
typedef uint8_t light_t;
typedef uint8_t alpha_t;

namespace constants
{
    const uint16_t PALETTE_ENTRIES    = 256;
    const uint16_t ALPHA_LEVELS       = 16;
    const uint16_t LIGHT_LEVELS       = 64;
    const uint16_t TEXTURE_SPACE_SIZE = 1024;
}

namespace functions
{
    int8_t getPowOf2(uint64_t number);
    bool   isPowOf2(uint64_t number);
    inline col_t *accessArray(col_t *buf, uint16_t x, uint16_t y, uint16_t rowLength)
    {
        return buf + x + y * rowLength;
    }
}

#endif // NAIVE_DEFS_HPP

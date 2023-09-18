#ifndef NAIVE_DEFS_HPP
#define NAIVE_DEFS_HPP

#include <cstdint>

namespace constants
{
    const uint16_t PALETTE_ENTRIES = 256;
    const uint16_t ALPHA_LEVELS = 16;
    const uint16_t LIGHT_LEVELS = 256;
}

namespace functions
{
    bool isPowOf2(uint64_t);
}

#endif // NAIVE_DEFS_HPP

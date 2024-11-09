#ifndef SCREEN_OPS_H
#define SCREEN_OPS_H

#include "screen.hpp"
#include <cstring>

namespace screenOps
{
    void copyScreen(Screen &source, Screen &dest)
    {
        if (source.width() == dest.width() && source.height() == dest.height())
        {
            std::memcpy(dest.pBuffer(), source.pBuffer(), source.height() * source.width() * sizeof(col_t));
        }
    }

    void copyRect()
    {

    }

    void clearScreen(Screen *dest, col_t col)
    {
        std::memset(dest->pBuffer(), col, dest->height() * dest->width() * sizeof(col_t));
    }
}

#endif

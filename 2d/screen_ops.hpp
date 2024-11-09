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
        if (dest->viewport().x() == 0 && dest->viewport().y() == 0
            && dest->viewport().w() == dest->width() && dest->viewport().h() == dest->height())
            std::memset(dest->pBuffer(), col, dest->height() * dest->width() * sizeof(col_t));
        else
        {
            col_t *startPtr = functions::accessArray(dest->pBuffer(), dest->viewport().x(), dest->viewport().y(), dest->width());
            size_t stride = dest->width();
            for (int y = dest->viewport().y(); y < dest->viewport().y() + dest->viewport().h(); ++y)
            {
                std::memset(dest->pBuffer(), col, dest->viewport().w());
                startPtr += stride;
            }
        }
    }
}

#endif

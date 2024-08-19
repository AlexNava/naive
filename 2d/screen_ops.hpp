#ifndef SCREEN_OPS_H
#define SCREEN_OPS_H

#include "screen.hpp"
#include <memory>

namespace screenOps
{
    void copyScreen(Screen &source, Screen &dest)
    {
        if (source.width() == dest.width() && source.height() == dest.height())
        {
            memcpy(dest.pBuffer(), source.pBuffer(), source.height() * source.width() * sizeof(col_t));
        }
    }

    void copyRect()
    {

    }


}

#endif

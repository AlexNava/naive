#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "naive_defs.hpp"

// todo use this class instead of having a pointer and two integers for size
class Screen
{
public:
    Screen(uint16_t width, uint16_t height, col_t *pBuffer = nullptr);
    ~Screen();

    uint16_t width() const;
    uint16_t height() const;

    inline void putPixel(uint16_t x, uint16_t y, col_t pixel)
    {
        // if valid?
        *(functions::accessArray(m_pBuffer, x, y, m_width)) = pixel;
    }
    inline col_t getPixel(uint16_t x, uint16_t y)
    {
        // if valid?
        return *(functions::accessArray(m_pBuffer, x, y, m_width));
    }

    col_t *pBuffer() const;

private:
    col_t *m_pBuffer;
    uint16_t m_width;
    uint16_t m_height;
    bool m_isValid;
    bool m_isBufferOwned;
};

#endif // SCREEN_HPP

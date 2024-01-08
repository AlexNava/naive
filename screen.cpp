#include "screen.hpp"

Screen::Screen(uint16_t width, uint16_t height, col_t *pBuffer)
{
    m_isBufferOwned = false;
    m_pBuffer = nullptr;
    m_isValid = false;

    if (width > 0 && height > 0)
    {
        if (pBuffer == nullptr)
        {
            pBuffer = new col_t[width * height];
            m_isBufferOwned = true;
        }

        m_pBuffer = pBuffer;
        m_width   = width;
        m_height  = height;
        m_isValid = true;
    }
    else
    {
        // error
    }
}

Screen::~Screen()
{
    if (m_isBufferOwned)
        delete[] m_pBuffer;
}

uint16_t Screen::width() const
{
    return m_width;
}

uint16_t Screen::height() const
{
    return m_height;
}

col_t *Screen::pBuffer() const
{
    return m_pBuffer;
}

#include "rasterizer.hpp"
#include "screen.hpp"

Rasterizer::Rasterizer() {
    m_subpixelEdges = false;
}

void Rasterizer::setPTargetScreen(Screen *newPTargetScreen)
{
    m_pTargetScreen = newPTargetScreen;
    if (m_pTargetScreen && m_pTargetScreen->height() > leftEdge.size())
    {
        leftEdge.resize(m_pTargetScreen->height());
    }
}

#ifndef RASTERIZER_HPP
#define RASTERIZER_HPP

#include <vector>
#include "naive_defs.hpp"

class Screen;

struct ScanlineEnd
{
    int32_t x;
    uint16_t textureU;
    uint16_t textureV;
    light_t luminance;
    uint8_t spare[3]; // alignment
};

class Rasterizer
{
public:
    Rasterizer();

    void setPTargetScreen(Screen *newPTargetScreen);

    void renderTriangle();

private:
    Screen *m_pTargetScreen;
    std::vector<ScanlineEnd> leftEdge;
    std::vector<ScanlineEnd> rightEdge;


    bool m_subpixelEdges;
};

#endif // RASTERIZER_HPP

#ifndef RASTERIZER_HPP
#define RASTERIZER_HPP

#include <thread>
#include <mutex>
#include <vector>
#include "naive_defs.hpp"

class Screen;
class Texture;
class Rasterizer;

struct RasterVertex
{
    rendCoord_t x;
    rendCoord_t y;
    rendDepth_t z;     // only used for perspective correction, if implemented
    uint16_t    u;
    uint16_t    v;
    light_t     light; // used for gouraud shading or flat shading
};

struct ScanlineEnd
{
    int32_t x;
    uint16_t textureU;
    uint16_t textureV;
    light_t luminance;
    uint8_t spare[3]; // alignment
};

struct WorkerData
{
    std::thread *pThread;
    Rasterizer  *pRasterizer;
    uint16_t     firstLine;
    uint16_t     lastLine;
    std::mutex  *pStartMutex;  // released by caller, locked by worker
    std::mutex  *pEndMutex;    // released by worker, locked by caller
    bool         stopRequested;
};

class Rasterizer
{
public:
    Rasterizer();
    ~Rasterizer();

    void setPTargetScreen(Screen *newPTargetScreen);

    void renderTriangle(RasterVertex *a, RasterVertex *b, RasterVertex *c, col_t flatColor, matFlags_t flags);

    Screen *pTargetScreen() const;
    void setPTexture(Texture *newPTexture);

private:
    Screen *m_pTargetScreen;
    Texture *m_pTexture;
    // @todo support texture units? fixed?
    // @todo define blend operations in texture unit parameters?
    //   they can be actually handled by triangle function by
    //   setting edges once and calling multiple scanlines
    // @todo make screen and texture classes related?
    //   alternative: function to copy rects for render to texture

    std::vector<ScanlineEnd> m_leftEdge;
    std::vector<ScanlineEnd> m_rightEdge;

    void calcScanlines(RasterVertex *a, RasterVertex *b, bool interpLight, bool interpUv);
    void calcEdge(RasterVertex *a, RasterVertex *b, bool interpLight, bool interpUv, std::vector<ScanlineEnd> &edge);

    // settable render flags
    bool m_subpixelEdges; // ignored

    std::vector<WorkerData*> m_threadPtrs;

    void startDrawingScanlines();
    void waitScanlines();

    void stopAllWorkers();
};

void scanlineWorker(WorkerData *);

#endif // RASTERIZER_HPP

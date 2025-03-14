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
    uint16_t    w;     // cube maps, maybe one day
    light_t     light; // used for gouraud shading or flat shading
    col_t       color; // used when no texturing is specified
};

struct ScanlineEnd
{
    int32_t x;
    int32_t textureU; // shifted
    int32_t textureV; // shifted
    int32_t textureW; // shifted
    int32_t luminance; // shifted
    //uint8_t spare[3]; // alignment
};

struct WorkerData
{
    std::thread *pThread;
    Rasterizer  *pRasterizer;
    ScanlineEnd *pLeftEdge;
    ScanlineEnd *pRightEdge;
    uint16_t     firstLine;
    uint16_t     lastLine;
    std::mutex  *pStartMutex;  // released by caller, locked by worker
    std::mutex  *pEndMutex;    // released by worker, locked by caller
    Texture     *pTexture;
    bool         stopRequested;
    uint8_t      workerNumber;
};

class Rasterizer
{
public:
    Rasterizer();
    ~Rasterizer();

    void setPTargetScreen(Screen *newPTargetScreen);

    void renderTriangle(RasterVertex *a, RasterVertex *b, RasterVertex *c, col_t solidColor, matFlags_t flags);

    Screen *pTargetScreen() const;
    void setPTexture(Texture *newPTexture);

    matFlags_t materialFlags() const;

    col_t solidColor() const;

    light_t flatLight() const;

private:
    Screen *m_pTargetScreen;
    Texture *m_pTexture;
    // @todo support texture units? fixed?
    // @todo define blend operations in texture unit parameters?
    //   they can be actually handled by triangle function by
    //   setting edges once and calling multiple scanlines
    // @todo make screen and texture classes related?
    //   alternative: function to copy rects for render to texture

    ScanlineEnd *m_pLeftEdge;
    ScanlineEnd *m_pRightEdge;
    int m_allocatedScanlines;

    void calcScanlines(RasterVertex *a, RasterVertex *b, bool interpLight, bool interpUv);
    void calcEdge(RasterVertex *a, RasterVertex *b, bool interpLight, bool interpUv, ScanlineEnd *edge);

    // settable render flags
    bool m_subpixelEdges; // ignored
    matFlags_t m_materialFlags;
    col_t m_solidColor;
    light_t m_flatLight;

    std::vector<WorkerData*> m_threadPtrs;

    void startDrawingScanlines();
    void waitScanlines();

    void stopAllWorkers();
};

void scanlineWorker(WorkerData *);

#endif // RASTERIZER_HPP

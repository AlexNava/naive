#ifndef RASTERIZER_HPP
#define RASTERIZER_HPP

#include <thread>
#include <mutex>
#include <vector>
#include "naive_defs.hpp"

class Screen;
class Texture;
class Rasterizer;

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
    uint16_t     firtLine;
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

    void renderTriangle();

    Screen *pTargetScreen() const;
    void setPTexture(Texture *newPTexture);

private:
    Screen *m_pTargetScreen;
    Texture *m_pTexture;
    // @todo support texture units? fixed?
    // @todo define blend operations in texture unit parameters?
    // @todo make scren and texture classes related?

    std::vector<ScanlineEnd> leftEdge;
    std::vector<ScanlineEnd> rightEdge;

    bool m_subpixelEdges;
    std::vector<WorkerData*> m_threadPtrs;

    void startDrawingScanlines();
    void waitScanlines();

    void stopAllWorkers();
};

void scanlineWorker(void *);

#endif // RASTERIZER_HPP

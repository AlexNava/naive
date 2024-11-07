#include <cmath>
#include "rasterizer.hpp"
#include "screen.hpp"
#include "texture.hpp"

static const int FP_SHIFT = 10;
static const int MAX_RASTER_THREADS = 4;

Rasterizer::Rasterizer()
{
    m_pLeftEdge = nullptr;
    m_pRightEdge = nullptr;
    m_subpixelEdges = false;

    // create threads
    m_threadPtrs.clear();
    for (int i = 0; i < MAX_RASTER_THREADS; ++i)
    {
        WorkerData* tempWkData = new WorkerData();
        tempWkData->pRasterizer = this;
        tempWkData->firstLine = 0;
        tempWkData->lastLine = 0;
        tempWkData->pStartMutex = new std::mutex();
        tempWkData->pStartMutex->lock();
        tempWkData->pEndMutex = new std::mutex();
        tempWkData->pEndMutex->lock();
        tempWkData->pThread = new std::thread(scanlineWorker, tempWkData);
        tempWkData->workerNumber = i + 1;
        m_threadPtrs.push_back(tempWkData);
    }

}

Rasterizer::~Rasterizer()
{
    // join threads
    stopAllWorkers();

    // delete allocated structs
    for (int i = 0; i < MAX_RASTER_THREADS; ++i)
    {
        delete m_threadPtrs.at(i);
    }

    delete[] m_pLeftEdge;
    delete[] m_pRightEdge;

}

void Rasterizer::setPTargetScreen(Screen *newPTargetScreen)
{
    m_pTargetScreen = newPTargetScreen;

    delete[] m_pLeftEdge;
    delete[] m_pRightEdge;
    m_pLeftEdge = new ScanlineEnd[m_pTargetScreen->height()];
    m_pRightEdge = new ScanlineEnd[m_pTargetScreen->height()];

    //if (m_pTargetScreen && m_pTargetScreen->height() > m_leftEdge.size())
    //{
    //    m_leftEdge.resize(m_pTargetScreen->height());
    //    m_rightEdge.resize(m_pTargetScreen->height());
    //}
}

void Rasterizer::renderTriangle(RasterVertex *a, RasterVertex *b, RasterVertex *c, col_t flatColor, matFlags_t flags)
{
    if (!a || !b || !c)
        return;

    bool flatShading = !(flags & materialFlags::GOURAUD)
            || (a->light == b->light && b->light == c->light);

    bool textured = (flags & materialFlags::TEXTURED); // todo pass to scanlines calc
    if (textured && (a->u != b->u || a->v != b->v || b->u != c->u || b->v != c->v))
    {
        textured = false;
        m_pTexture->setMipLevel(0);
        flatColor = m_pTexture->getTexel(a->u, a->v);
    }

    // calculate edges
    if (m_subpixelEdges)
    {
        // to be implemented later
    }
    else
    {
        calcScanlines(a, b, !flatShading, textured);
        calcScanlines(b, c, !flatShading, textured);
        calcScanlines(c, a, !flatShading, textured);
    }

    // setup workers data, try to balance load
    int minY = a->y;
    int maxY = minY;

    if (b->y < minY)
        minY = b->y;
    if (b->y > maxY)
        maxY = b->y;
    if (c->y < minY)
        minY = c->y;
    if (c->y > maxY)
        maxY = c->y;
    if (minY < m_pTargetScreen->viewport().y())
        minY = m_pTargetScreen->viewport().y();
    if (maxY >= m_pTargetScreen->viewport().y() + m_pTargetScreen->viewport().h())
        maxY = m_pTargetScreen->viewport().y() + m_pTargetScreen->viewport().h() - 1;

    // not optimal, simply divide y span by number of threads
    for (int i = 0; i < MAX_RASTER_THREADS; ++i)
    {
        if (i == 0)
        {
            m_threadPtrs[i]->firstLine = minY;
            m_threadPtrs[i]->lastLine = minY + (maxY - minY) / MAX_RASTER_THREADS;
        }
        else if (i == MAX_RASTER_THREADS - 1)
        {
            m_threadPtrs[i]->firstLine = m_threadPtrs[i - 1]->lastLine + 1;
            m_threadPtrs[i]->lastLine = maxY;
        }
        else
        {
            m_threadPtrs[i]->firstLine = m_threadPtrs[i - 1]->lastLine + 1;
            m_threadPtrs[i]->lastLine = minY + (maxY - minY) * (i + 1) / MAX_RASTER_THREADS;
        }
    }

    // draw scanlines in parallel
    startDrawingScanlines();
    waitScanlines();
}

void Rasterizer::calcScanlines(RasterVertex *a, RasterVertex *b, bool interpLight, bool interpUv)
{
    // pointers assumed valid because they are checked in the only calling function
    // triangles are drawn if vertices appear clockwise
    // horizontally aligned vertices don't make edges

    if (b->y > a->y)
    {
        calcEdge(a, b, interpLight, interpUv, m_pRightEdge);
    }
    else if (b->y < a->y)
    {
        calcEdge(b, a, interpLight, interpUv, m_pLeftEdge);
    }
}

void Rasterizer::calcEdge(RasterVertex *a, RasterVertex *b, bool interpLight, bool interpUv, ScanlineEnd *edge)
{
    bool clearEdge = false;
    int32_t startLine = a->y;
    int32_t endLine = b->y;
    int32_t deltaX = b->x - a->x;
    int32_t deltaY = b->y - a->y; // always positive
    uint32_t xSlope = std::abs(deltaX);
    int32_t x = a->x << FP_SHIFT;

    if (xSlope >= (1 << (32 - FP_SHIFT)))
    {
        clearEdge = true;
        xSlope = 0;
    }
    else
    {
        xSlope = xSlope << FP_SHIFT;
        xSlope = xSlope / deltaY;
    }

    if (a->y < 0)
    {
        if (deltaX >= 0)
            x += xSlope * (- a->y);
        else
            x -= xSlope * (- a->y);
        startLine = 0;
    }

    if (a->y >= m_pTargetScreen->height())
    {
        endLine = m_pTargetScreen->height() - 1;
    }

    for (int i = startLine; i <= endLine; ++i)
    {
        // set edge
        edge[i].x = x >> FP_SHIFT;
        if (deltaX >= 0)
            x += xSlope;
        else
            x -= xSlope;

        // interp light

        // interp uv
    }
}

Screen *Rasterizer::pTargetScreen() const
{
    return m_pTargetScreen;
}

void Rasterizer::setPTexture(Texture *newPTexture)
{
    m_pTexture = newPTexture;
}

void Rasterizer::startDrawingScanlines()
{
    for (unsigned int i = 0; i < m_threadPtrs.size(); ++i)
    {
        m_threadPtrs.at(i)->pStartMutex->unlock();
    }
}

void Rasterizer::waitScanlines()
{
    for (unsigned int i = 0; i < m_threadPtrs.size(); ++i)
    {
        m_threadPtrs.at(i)->pEndMutex->lock();
    }
}

void Rasterizer::stopAllWorkers()
{
    for (unsigned int i = 0; i < m_threadPtrs.size(); ++i)
    {
        m_threadPtrs.at(i)->stopRequested = true;
        m_threadPtrs.at(i)->pStartMutex->unlock();
        m_threadPtrs.at(i)->pThread->join();
    }
}

void scanlineWorker(WorkerData *pWkData)
{
    Rasterizer *pRasterizer = pWkData->pRasterizer;
    uint16_t scanline = 0;

    while (true)
    {
        pWkData->pStartMutex->lock();

        if (pWkData->stopRequested)
            break;

        for (scanline = pWkData->firstLine; scanline <= pWkData->lastLine; ++scanline)
        {
            // draw scanlines
            for (int x = pRasterizer->m_pLeftEdge[scanline].x; x < pRasterizer->m_pRightEdge[scanline].x; ++x)
            {
                pRasterizer->pTargetScreen()->putPixel(x, scanline, 60 + pWkData->workerNumber);
            }
        }

        pWkData->pEndMutex->unlock();
    }
}

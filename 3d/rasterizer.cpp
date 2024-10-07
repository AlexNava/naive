#include "rasterizer.hpp"
#include "screen.hpp"

Rasterizer::Rasterizer()
{
    const int MAX_RASTER_THREADS = 4;

    m_subpixelEdges = false;

    // create threads
    m_threadPtrs.clear();
    for (int i = 0; i < MAX_RASTER_THREADS; ++i)
    {
        WorkerData* tempWkData = new WorkerData();
        tempWkData->pRasterizer = this;
        tempWkData->firtLine = 0;
        tempWkData->lastLine = 0;
        tempWkData->pStartMutex = new std::mutex();
        tempWkData->pStartMutex->lock();
        tempWkData->pEndMutex = new std::mutex();
        tempWkData->pEndMutex->lock();
        tempWkData->pThread = new std::thread((void *)scanlineWorker, tempWkData);
        m_threadPtrs.push_back(tempWkData);
    }

}

Rasterizer::~Rasterizer()
{
    // join threads
    stopAllWorkers();
}

void Rasterizer::setPTargetScreen(Screen *newPTargetScreen)
{
    m_pTargetScreen = newPTargetScreen;
    if (m_pTargetScreen && m_pTargetScreen->height() > leftEdge.size())
    {
        leftEdge.resize(m_pTargetScreen->height());
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
    for (int i = 0; i < m_threadPtrs.size(); ++i)
    {
        m_threadPtrs.at(i)->pStartMutex->unlock();
    }
}

void Rasterizer::waitScanlines()
{
    for (int i = 0; i < m_threadPtrs.size(); ++i)
    {
        m_threadPtrs.at(i)->pEndMutex->lock();
    }
}

void Rasterizer::stopAllWorkers()
{
    for (int i = 0; i < m_threadPtrs.size(); ++i)
    {
        m_threadPtrs.at(i)->stopRequested = true;
        m_threadPtrs.at(i)->pStartMutex->unlock();
        m_threadPtrs.at(i)->pThread->join();
    }
}

void scanlineWorker(void *pWkData)
{
    WorkerData *pData = (WorkerData*) pWkData;
    Rasterizer *pRasterizer = pData->pRasterizer;
    uint16_t scanline = 0;

    while (true)
    {
        pData->pStartMutex->lock();

        if (pData->stopRequested)
            break;

        for (scanline = pData->firtLine; scanline <= pData->lastLine; ++scanline)
        {
            // draw scanlines
        }

        pData->pEndMutex->unlock();
    }
}

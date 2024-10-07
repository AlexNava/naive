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
        tempWkData->pThread = new std::thread(scanlineWorker, tempWkData);
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

        for (scanline = pWkData->firtLine; scanline <= pWkData->lastLine; ++scanline)
        {
            // draw scanlines
        }

        pWkData->pEndMutex->unlock();
    }
}

#include <cmath>
#include <cstdio>

#include "rasterizer.hpp"
#include "screen.hpp"
#include "texture.hpp"
#include "palette.hpp"

static const int FP_SHIFT = 8;
static const int MAX_RASTER_THREADS = 2;

Rasterizer::Rasterizer()
{
    m_pLeftEdge = new ScanlineEnd[1000];
    m_pRightEdge = new ScanlineEnd[1000];
    m_allocatedScanlines = 1000;
    m_pTargetScreen = nullptr;

    m_subpixelEdges = false;

    // create threads
    m_threadPtrs.clear();
    for (int i = 0; i < MAX_RASTER_THREADS; ++i)
    {
        WorkerData* tempWkData = new WorkerData();
        tempWkData->pRasterizer = this;
        tempWkData->firstLine = 0;
        tempWkData->lastLine = 0;
        tempWkData->pLeftEdge = m_pLeftEdge;
        tempWkData->pRightEdge = m_pRightEdge;
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

    if (m_pTargetScreen && m_pTargetScreen->height() > m_allocatedScanlines)
    {
        m_allocatedScanlines = m_pTargetScreen->height();
        delete[] m_pLeftEdge;
        delete[] m_pRightEdge;
        m_pLeftEdge = new ScanlineEnd[m_allocatedScanlines];
        m_pRightEdge = new ScanlineEnd[m_allocatedScanlines];
        for (int i = 0; i < MAX_RASTER_THREADS; ++i)
        {
            m_threadPtrs[i]->pLeftEdge = m_pLeftEdge;
            m_threadPtrs[i]->pRightEdge = m_pRightEdge;
        }
    }
}

void Rasterizer::renderTriangle(RasterVertex *a, RasterVertex *b, RasterVertex *c, col_t solidColor, matFlags_t flags)
{
    if (!a || !b || !c)
        return;

    m_materialFlags = flags;

    bool flatShading = !(flags & materialFlags::SOFT_SHADED)
            || (a->light == b->light && b->light == c->light);

    bool textured = (flags & materialFlags::TEXTURED); // todo pass to scanlines calc
    if (textured && a->u == b->u && a->v == b->v && b->u == c->u && b->v == c->v)
    { // @todo rethink this
        textured = false;
        m_materialFlags &= ~materialFlags::TEXTURED;
        m_pTexture->setMipLevel(0);
        solidColor = m_pTexture->getTexel(a->u, a->v);
    }

    m_solidColor = solidColor;
    m_flatLight = a->light;

//    printf("    starting calc scanlines. vertices: a %d, %d; b %d, %d; c %d, %d\n",
//           a->x, a->y, b->x, b->y, c->x, c->y);

    // calculate edges
    //if (m_subpixelEdges)
    //{
    //    // to be implemented later
    //}
    //else
    {
        calcScanlines(a, b, !flatShading, textured);
        calcScanlines(b, c, !flatShading, textured);
        calcScanlines(c, a, !flatShading, textured);
    }

//    printf("    starting load balance\n");
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

//    printf("   workers setup. lines: ");
//    for (int i = 0; i < m_threadPtrs.size(); ++i)
//    {
//        printf("%d -> %d ; ", m_threadPtrs[i]->firstLine, m_threadPtrs[i]->lastLine);
//    }
//    printf("\n");

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
    int32_t xSlope = deltaX << FP_SHIFT;
    int32_t x = a->x << FP_SHIFT;

    uint32_t u = a->u << FP_SHIFT;
    uint32_t v = a->v << FP_SHIFT;
    int32_t deltaU;
    int32_t deltaV;
    if (interpUv)
    {
        deltaU = ((int32_t)(b->u - a->u) << FP_SHIFT) / deltaY;
        deltaV = ((int32_t)(b->v - a->v) << FP_SHIFT) / deltaY;
    }

    uint32_t light = a->light << FP_SHIFT;
    int32_t deltaL;
    if (interpLight)
        deltaL = ((int32_t)(b->light - a->light) << FP_SHIFT) / deltaY;

    if (xSlope * deltaX < 0) // sign changed by  shift
    {
        clearEdge = true;
        xSlope = 0;
    }
    else
    {
        xSlope = xSlope / deltaY;
    }

    if (a->y < 0)
    {
        x     += xSlope * (- a->y);
        if (interpUv)
        {
            u     += deltaU * (- a->y);
            v     += deltaV * (- a->y);
        }
        if (interpLight)
            light += deltaL * (- a->y);

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
        x += xSlope;

        // interp uv
        if (interpUv)
        {
            edge[i].textureU = u;
            edge[i].textureV = v;
            u += deltaU;
            v += deltaV;
        }
        // interp light
        if (interpLight)
        {
            edge[i].luminance = light;
            light += deltaL;
        }
    }
}

light_t Rasterizer::flatLight() const
{
    return m_flatLight;
}

col_t Rasterizer::solidColor() const
{
    return m_solidColor;
}

matFlags_t Rasterizer::materialFlags() const
{
    return m_materialFlags;
}

Screen *Rasterizer::pTargetScreen() const
{
    return m_pTargetScreen;
}

void Rasterizer::setPTexture(Texture *newPTexture)
{
    m_pTexture = newPTexture;
    for (int i = 0; i < m_threadPtrs.size(); ++i)
    {
        m_threadPtrs[i]->pTexture = m_pTexture;
    }
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
    Screen *pTargetScreen;
    Palette &palette = Palette::getInstance();
    uint16_t line = 0;
    int32_t u, v;
    int32_t uStep, vStep;
    int32_t l;
    int32_t lStep;
    int32_t span;
    col_t fgPixel, bgPixel;
    int16_t luminance;
    int16_t lightSpaceShift = functions::getPowOf2(constants::LIGHT_SPACE_SIZE / constants::LIGHT_LEVELS);
    matFlags_t matFlags;
    bool isTextured;
    col_t solidColor;
    bool isLighted;
    bool isLightFlat;
    light_t flatLight;
    printf("worker %d created\n", pWkData->workerNumber);

    while (true)
    {
        pWkData->pStartMutex->lock();

//        printf("worker %d start obtained. lines %d to %d\n", pWkData->workerNumber, pWkData->firstLine, pWkData->lastLine);

        if (pWkData->stopRequested)
            break;

        pTargetScreen = pRasterizer->pTargetScreen();
        matFlags = pRasterizer->materialFlags();

        isTextured = matFlags & materialFlags::TEXTURED;
        if (!isTextured)
            solidColor = pRasterizer->solidColor();

        isLighted = (matFlags & (materialFlags::SHADED | materialFlags::SOFT_SHADED));

        isLightFlat = !(matFlags & materialFlags::SOFT_SHADED);
        if (isLightFlat)
            flatLight = pRasterizer->flatLight();

        if (pTargetScreen && pWkData->pLeftEdge && pWkData->pRightEdge)
            for (line = pWkData->firstLine; line <= pWkData->lastLine; ++line)
            {
                // draw scanlines
                span = pWkData->pRightEdge[line].x - pWkData->pLeftEdge[line].x;
                span += !span;

                if (isTextured)
                {
                    u = pWkData->pLeftEdge[line].textureU;
                    uStep = (pWkData->pRightEdge[line].textureU - u) / span;
                    v = pWkData->pLeftEdge[line].textureV;
                    vStep = (pWkData->pRightEdge[line].textureV - v) / span;
                }

                if (isLighted)
                {
                    l = pWkData->pLeftEdge[line].luminance;
                    lStep = (pWkData->pRightEdge[line].luminance - l) / span;
                }

                for (int x = pWkData->pLeftEdge[line].x; x < pWkData->pRightEdge[line].x; ++x)
                {
                    // sample background pixel
                    if (matFlags & (materialFlags::ADD_TRANSPARENT | materialFlags::TRANSPARENT))
                        bgPixel = pTargetScreen->getPixel(x, line);

                    // sample u v >> FP_SHIFT
                    if (isTextured)
                    {
                        fgPixel = pWkData->pTexture->getTexel(u >> FP_SHIFT, v >> FP_SHIFT);
                        u += uStep;
                        v += vStep;
                    }
                    else
                    {
                        fgPixel = solidColor;
                    }

                    if (isLighted)
                    {
                        if (isLightFlat)
                        {
                            luminance = flatLight; // necessary to reassign for dithering
                        }
                        else
                        {
                            luminance = l >> FP_SHIFT;
                            l += lStep;
                        }

                        luminance += functions::getDither(x, line);
                        if (luminance >= constants::LIGHT_SPACE_SIZE)
                            luminance = constants::LIGHT_SPACE_SIZE - 1;
                        if (luminance < 0)
                            luminance = 0;


                        fgPixel = palette.getLightedColor(fgPixel, luminance >> lightSpaceShift);
                    }

                    if (matFlags & materialFlags::TRANSPARENT)
                        fgPixel = palette.getBlendedColor(fgPixel, bgPixel, 7);
                    else if (matFlags & materialFlags::ADD_TRANSPARENT)
                        fgPixel = palette.getAddedColor(fgPixel, bgPixel);

                    pTargetScreen->putPixel(x, line, fgPixel);
                }
            }

//        printf("worker %d signaling done\n", pWkData->workerNumber);

        pWkData->pEndMutex->unlock();
    }
}

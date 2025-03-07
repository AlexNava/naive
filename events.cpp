#include "events.hpp"

Events &Events::getInstance()
{
    static Events singleton;
    return singleton;
}

void Events::poll()
{
    SDL_Event evt;
    while (SDL_PollEvent(&evt))
    {
        if (evt.type == SDL_QUIT)
        {
            m_isQuitRequested = true;
        }
    }

    //SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

}

Events::Events()
{
    m_isQuitRequested = false;
}

bool Events::isQuitRequested() const
{
    return m_isQuitRequested;
}

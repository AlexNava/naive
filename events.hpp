#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <SDL2/SDL_events.h>

class Events
{
public:
    static Events &getInstance();

    void poll();

    bool isQuitRequested() const;

private:
    Events();

    bool m_isQuitRequested;
};

#endif // EVENTS_HPP

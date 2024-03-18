#include "renderer.hpp"
#include "glm/glm.hpp"
#include "screen.hpp"

Renderer::Renderer(Screen *targetScreen)
{
    m_pScreen = targetScreen;
}

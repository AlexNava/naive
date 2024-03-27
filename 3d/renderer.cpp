#include "renderer.hpp"
#include "glm/glm.hpp"
#include "screen.hpp"
#include "scene.hpp"

Renderer::Renderer(Screen *targetScreen)
{
    m_pScreen = targetScreen;
}

void Renderer::setPScene(Scene *newPScene)
{
    m_pScene = newPScene;
}

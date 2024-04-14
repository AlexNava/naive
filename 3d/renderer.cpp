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

void Renderer::render()
{
    // query distance of objects in the scene
    // sort objects
    // draw all of them
    //   > get sorted list of triangles
    //   > drawtaking flags into account
}

void Renderer::drawScene()
{

}

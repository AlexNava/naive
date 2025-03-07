#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "naive_defs.hpp"

class Scene;
class Screen;
class Obj3D;

class Renderer
{
public:
    Renderer(Screen *targetScreen);

    void setPScene(Scene *newPScene);

    void render();

private:
    Screen *m_pScreen;
    Scene *m_pScene;
};

#endif // RENDERER_HPP

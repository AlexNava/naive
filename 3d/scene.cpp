#include "scene.hpp"

Scene::Scene()
{

}

void Scene::addObject(Obj3D *obj, Obj3D *parent, std::string name)
{
    if (name.size() == 0)
    {
        name = "auto_obj_" + std::to_string(m_objects.size());
    }

}

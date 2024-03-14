#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <map>
#include "obj3d.hpp"

class Scene
{
public:
    Scene();
protected:
    std::map<std::string, Obj3D> m_objects;
};

#endif // SCENE_HPP

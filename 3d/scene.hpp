#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <map>
#include <glm/mat4x4.hpp>

class Obj3D;

struct SceneObj
{
    Obj3D *pObject;
    Obj3D *pParent;
    glm::mat4x4 transformation;
    glm::mat4x4 worldTransformation; // only computed
    bool worldTransValid;
};

class Scene
{
public:
    Scene();
    void addObject(Obj3D *obj, Obj3D *parent = nullptr, std::string name = "");
protected:
    std::map<std::string, Obj3D*> m_objects;
};

#endif // SCENE_HPP

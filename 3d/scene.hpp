#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>

#include <vector>
#include <map>
#include <glm/mat4x4.hpp>

#include "naive_defs.hpp"
#include "scenenode.hpp"

class Obj3D;

class Scene
{
public:
    Scene();
    void addObjectAsNode(Obj3D *obj, SceneNode *parent = nullptr, std::string name = "");

    glm::vec3 viewDirection() const;

protected:
    std::vector<SceneNode*> m_objects;
    std::map<std::string, SceneNode*> m_objectsMap;
    // todo store

    std::vector<uint16_t> m_sortedIndexes;
    glm::vec3 m_viewPoint;     // world frame
    glm::vec3 m_viewDirection; // world frame. pass this to objects to get (pre)sorted tris list
    void sortObjects();

};

#endif // SCENE_HPP

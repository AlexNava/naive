#include "scene.hpp"
#include "scenenode.hpp"

Scene::Scene()
{

}

/// Add a new SceneNode to the scene containing the passed Obj3D
/// The scene will have ownership of the Obj3D and is responsible to delete it
///
void Scene::addObjectAsNode(Obj3D *obj, SceneNode *parent, std::string name)
{    
    if (name.size() == 0)
    {
        name = "auto_obj_" + std::to_string(m_objects.size());
    }

    SceneNode *pObjToAdd = new SceneNode();

    pObjToAdd->setPObject(obj);
    pObjToAdd->setPParent(parent);

    m_objects.push_back(pObjToAdd);
    m_objectsMap[name] = pObjToAdd;
}

glm::vec3 Scene::viewDirection() const
{
    return m_viewDirection;
}

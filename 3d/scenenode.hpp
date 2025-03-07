#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <glm/matrix.hpp>
#include "naive_defs.hpp"

class Obj3D;

class SceneNode
{
public:
    SceneNode();
    void setPParent(SceneNode *newPParent);

    void setPObject(Obj3D *newPObject);

private:
    Obj3D *pObject;
    SceneNode *pParent;
    glm::mat4x4 transformation;
    glm::mat4x4 worldTransformation; // only computed
    bool worldTransValid;
    rendFlags_t renderHints;
};

#endif // SCENENODE_HPP

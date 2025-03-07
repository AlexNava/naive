#include "scenenode.hpp"

SceneNode::SceneNode() {}

void SceneNode::setPParent(SceneNode *newPParent)
{
    pParent = newPParent;
}

void SceneNode::setPObject(Obj3D *newPObject)
{
    pObject = newPObject;
}

#ifndef OBJ3D_HPP
#define OBJ3D_HPP

#include <vector>
#include "glm/mat4x4.hpp"

class Obj3D
{
public:
    Obj3D();
private:
    glm::mat4x4 m_transformation;
    glm::mat4x4 m_worldTransformation; // only computed
    std::vector<Obj3D*> m_children;
    Obj3D *m_pParent;
};

#endif // OBJ3D_HPP

#ifndef MESH_HPP
#define MESH_HPP

#include "obj3d.hpp"
#include <glm/vec3.hpp>

struct triangle
{
    uint32_t vertexId[3];
    uint32_t normalId[3];
    uint32_t texUvId[3];
};

class Mesh : public Obj3D
{
public:
    Mesh();
private:
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_texUv;
    // todo optimization: add sorting for n directions?
};

#endif // MESH_HPP

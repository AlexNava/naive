#ifndef MESH_HPP
#define MESH_HPP

#include "obj3d.hpp"
#include <glm/vec3.hpp>

typedef uint16_t flags_t;

namespace renderFlags
{
const flags_t transparent = 1;
const flags_t gouraud     = 1 << 1;
const flags_t reflection  = 1 << 2;
}

class Mesh : public Obj3D
{
    friend class ObjLoader;

    struct Triangle
    {
        uint32_t vertexId[3];
        uint32_t normalId[3];
        uint32_t texUvId[3];
        flags_t flags;
    };

public:
    Mesh();
private:
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_texUv;

    std::vector<Mesh::Triangle> m_triangles;
    // todo optimization: add sorting for n directions?
};

#endif // MESH_HPP

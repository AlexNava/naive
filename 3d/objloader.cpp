#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_DOUBLE
#include "tiny_obj_loader.h"

#include "objloader.hpp"

#include <glm/vec3.hpp>
#include "mesh.hpp"

ObjLoader &ObjLoader::getInstance()
{
    static ObjLoader singleton;
    return singleton;
}

Mesh *ObjLoader::loadMesh(std::string fileName)
{
    Mesh *pResult = nullptr;

    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string errorStr;
    std::string warningStr;
    bool loadSuccess = tinyobj::LoadObj(&attributes, &shapes, &materials, &errorStr, &warningStr, fileName.c_str());

    if (loadSuccess)
    {
        std::vector<glm::vec3> vertices;
        auto xyzData = attributes.GetVertices();
        for (int iVert = 0; iVert <= xyzData.size() - 3; iVert += 3)
        {
            glm::vec3 vert;
            vert.x = xyzData.at(iVert);
            vert.y = xyzData.at(iVert + 1);
            vert.z = xyzData.at(iVert + 2);
            vertices.push_back(vert);
        }

        vertices;
    }

    return pResult;
}

ObjLoader::ObjLoader() {}

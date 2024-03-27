#include "objloader.hpp"
#include "mesh.hpp"

ObjLoader &ObjLoader::getInstance()
{
    static ObjLoader singleton;
    return singleton;
}

Mesh *ObjLoader::loadMesh(std::string fileName)
{
    Mesh *pResult = new Mesh();

    return pResult;
}

ObjLoader::ObjLoader() {}

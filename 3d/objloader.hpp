#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include <string>

class Obj3D;
class Mesh;

class ObjLoader
{
public:
    ObjLoader &getInstance();
    Mesh *loadMesh(std::string fileName);
private:
    ObjLoader();
};

#endif // OBJLOADER_HPP

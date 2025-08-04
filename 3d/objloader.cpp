#include "objloader.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_USE_DOUBLE
#include "tiny_obj_loader.h"

#include <iostream>
#include <glm/vec3.hpp>
#include "mesh.hpp"

ObjLoader &ObjLoader::getInstance()
{
    static ObjLoader singleton;
    return singleton;
}

Mesh *ObjLoader::loadMesh(std::string fileName)
{
    Mesh *pResult = new Mesh();//nullptr;

    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files
    if (!reader.ParseFromFile(fileName, reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        //exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // temp storage for mesh data
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<Mesh::Triangle> triangles;
    Mesh::Triangle triangle;

    // Loop through all vertices
    auto fileVerts = attrib.vertices;
    for (unsigned int iVert = 0; iVert <= fileVerts.size() - 3; iVert += 3)
    {
        glm::vec3 vert;
        vert.x = fileVerts.at(iVert);
        vert.y = fileVerts.at(iVert + 1);
        vert.z = fileVerts.at(iVert + 2);
        vertices.push_back(vert);
    }
    pResult->m_vertices = vertices;

    // Loop through all normals
    auto fileNorms = attrib.normals;
    for (unsigned int iNorm = 0; iNorm <= fileNorms.size() - 3; iNorm += 3)
    {
        glm::vec3 norm;
        norm.x = fileNorms.at(iNorm);
        norm.y = fileNorms.at(iNorm + 1);
        norm.z = fileNorms.at(iNorm + 2);
        normals.push_back(norm);
    }
    pResult->m_normals = normals;

    // Loop through all UVs
    auto fileUvs = attrib.texcoords;
    for (unsigned int iUv = 0; iUv <= fileUvs.size() - 2; iUv += 2)
    {
        glm::vec2 uv;
        uv.x = fileUvs.at(iUv);
        uv.y = fileUvs.at(iUv + 1);
        uvs.push_back(uv);
    }
    pResult->m_texUv = uvs;

    // Loop over shapes. ONLY ONE
    for (size_t s = 0; s < (shapes.size() > 0); s++)
    {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            if (fv == 3)
                for (size_t v = 0; v < fv; v++)
                {
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                    tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                    tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];                    
                    triangle.vertexId[v] = idx.vertex_index;

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    if (idx.normal_index >= 0) {
                        tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                        tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                        tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                    }
                    triangle.normalId[v] = idx.normal_index;

                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    if (idx.texcoord_index >= 0) {
                        tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                        tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
                    }
                    triangle.texUvId[v] = idx.texcoord_index;

                    // Optional: vertex colors
                    // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                    // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                    // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
                }

            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];

            triangles.push_back(triangle);
        }
    }

    pResult->m_triangles = triangles;

/*
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
        for (unsigned int iVert = 0; iVert <= xyzData.size() - 3; iVert += 3)
        {
            glm::vec3 vert;
            vert.x = xyzData.at(iVert);
            vert.y = xyzData.at(iVert + 1);
            vert.z = xyzData.at(iVert + 2);
            vertices.push_back(vert);
        }
        pResult->m_vertices = vertices;

        for (unsigned int iVert = 0; iVert <= xyzData.size() - 3; iVert += 3)
        {

        }
        shapes[0].mesh.indices[0];


        // shapes[0].mesh.indices(36 = 3*12 faces).normal/texcoord/vertex
    }
    else
    {
        delete pResult;
        pResult = nullptr;
    }
*/
    return pResult;
}

ObjLoader::ObjLoader() {}

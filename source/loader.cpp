#define TINYOBJLOADER_IMPLEMENTATION
#include "loader.h"
#include <iostream>

void Loader::parseFile(string fileName) 
{
    tinyobj::ObjReaderConfig readerConfig;
    readerConfig.mtl_search_path = "./object_files";

    tinyobj::ObjReader reader;

    if(!reader.ParseFromFile(fileName, readerConfig)) {
        // If reader detects known error.
        if (!reader.Error().empty()) {
            cout << "\nError: \n";
            cout << "\tTinyObjReader: " << reader.Error();
            objectLoadError = true;
        }
        // If reader is unable to parse the file.
        return;
    }

    if (!reader.Warning().empty()) {
        cout << "\nWarning: \n";
        cout << "\tTinyObjReader: " << reader.Warning();
    }
    
    objectLoadError = false;
    loadedFile = fileName;
    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    //auto& materials = reader.GetMaterials();
    
    // Allocate the number of shapes.
    vertexCoords.resize(shapes.size());
    indices.resize(shapes.size());
    textureCoords.resize(shapes.size());
    colorVals.resize(shapes.size());
    vertexNormals.resize(shapes.size());
    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {

        size_t index_offset = 0;
        size_t vOffset = 0;
        std::vector<unsigned char> faceVertices = shapes[s].mesh.num_face_vertices;

        // Store all vertex coordinates
        for (size_t i = 0; i < attrib.vertices.size(); i+=3) {
                vertexCoords[s].push_back(glm::vec3(attrib.vertices[i], attrib.vertices[i+1], attrib.vertices[i+2]));
        }

        // Loop over faces(polygon)
        for (size_t f = 0; f < faceVertices.size(); f++) {
            size_t fv = size_t(faceVertices[f]);

            // Store all indices for each face
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                indices[s].push_back(idx.vertex_index);
            }
            
            // Check if `normal_index` is zero or positive. negative = no normal data
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                if (idx.normal_index >= 0) {
                    vOffset = 3*size_t(idx.normal_index);
                    vertexNormals[s].push_back(glm::vec3(attrib.normals[vOffset], attrib.normals[vOffset+1], attrib.normals[vOffset+2]));
                }
            }

            // Check if `texcoord_index` is zero or positive. negative = no texcoord data
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                if (idx.texcoord_index >= 0) {
                    vOffset = 2*size_t(idx.texcoord_index);
                    textureCoords[s].push_back(glm::vec3(attrib.texcoords[vOffset], attrib.texcoords[vOffset+1], attrib.texcoords[vOffset+2]));
                }
            }

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                vOffset = 3*size_t(idx.vertex_index);
                colorVals[s].push_back(glm::vec3(attrib.colors[vOffset], attrib.colors[vOffset+1], attrib.colors[vOffset+2]));
            }

            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
}

void Loader::normalizeCoords()
{
    float largest_length = 0;
    float new_length;
    for(size_t s = 0; s < vertexCoords[0].size(); s++)
    {
        new_length = sqrt(pow(vertexCoords[0][s].x, 2.0) + pow(vertexCoords[0][s].y, 2.0) + pow(vertexCoords[0][s].z, 2.0));
        if(largest_length < new_length) {
            largest_length = new_length; 
        }    
    }

    for(size_t s = 0; s < vertexCoords[0].size(); s++)
    {
        vertexCoords[0][s] /= largest_length;    
    }

}

void Loader::clearLoader()
{
    vertexCoords.clear();
    indices.clear();
    vertexNormals.clear();
    textureCoords.clear();
    colorVals.clear();
}
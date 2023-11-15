#define TINYOBJLOADER_IMPLEMENTATION
#include "loader.h"
#include <iostream>

/**
 * Parses a given object file and stores the different values in the loader class.
 * 
 * The function uses the tiny_obj_loader.h (https://github.com/tinyobjloader/tinyobjloader) 
 * to handle the inital parsing of the file.
 * 
 * Attributes effected by method:
 *      vertexCoords    - The all vertex coordinates for a shape.
 *      indices         - The index of vertex coordinates for each face of a shape.
 *      textureCoords   - The texture coordinates for each face of a shape.
 *      colorValues     - The color values for each face of a shape.
 *      vertexNormals   - The vertex normals for each face of a shape.
 *      objectLoadError - Returns a true/false if a error has occured when trying to parse the file.
 *  
 * If any errors occur corresponding output will be sent without crashing the program.
 * 
 * @returns true/false if the given object file could be parsed.
 */
bool Loader::parseFile(string fileName) 
{
    tinyobj::ObjReaderConfig readerConfig;
    readerConfig.mtl_search_path = "./object_files";
    tinyobj::ObjReader reader;

    if(!reader.ParseFromFile(fileName, readerConfig)) {
        // If reader detects known error.
        if (!reader.Error().empty()) {
            cout << "\nError: \n";
            cout << "\tTinyObjReader: " << reader.Error();
        }
        // If reader is unable to parse the file.
        return false;
    }

    if (!reader.Warning().empty()) {
        cout << "\nWarning: \n";
        cout << "\tTinyObjReader: " << reader.Warning();
    }
    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    //auto& materials = reader.GetMaterials();
    
    // Allocate the number of shapes for the data vectors.
    vertexCoords.resize(shapes.size());
    indices.resize(shapes.size());
    textureCoords.resize(shapes.size());
    colorVals.resize(shapes.size());
    vertexNormals.resize(shapes.size());

    // Loop over object shapes
    for (size_t s = 0; s < shapes.size(); s++) {

        size_t index_offset = 0;
        size_t vOffset = 0;
        std::vector<unsigned char> faceVertices = shapes[s].mesh.num_face_vertices;

        // Store all vertex coordinates
        for (size_t i = 0; i < attrib.vertices.size(); i+=3) {
                vertexCoords[s].push_back(glm::vec3(
                    attrib.vertices[i], 
                    attrib.vertices[i+1], 
                    attrib.vertices[i+2]));
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
                    vertexNormals[s].push_back(glm::vec3(
                        attrib.normals[vOffset], 
                        attrib.normals[vOffset+1], 
                        attrib.normals[vOffset+2]));
                }
            }

            // Check if `texcoord_index` is zero or positive. negative = no texcoord data
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                if (idx.texcoord_index >= 0) {
                    vOffset = 2*size_t(idx.texcoord_index);
                    textureCoords[s].push_back(glm::vec3(
                        attrib.texcoords[vOffset], 
                        attrib.texcoords[vOffset+1], 
                        attrib.texcoords[vOffset+2]));
                }
            }

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                vOffset = 3*size_t(idx.vertex_index);
                colorVals[s].push_back(glm::vec3(
                    attrib.colors[vOffset], 
                    attrib.colors[vOffset+1], 
                    attrib.colors[vOffset+2]));
            }

            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
        numberOfObjects++;
    }
    return true;
}

/**
 * Normalizes all the shapes coordinates to fit inside the NDC cube.
 * This will change the vertexCoords value in the loader class.
 */
void Loader::normalizeVertexCoords()
{
    for(size_t s = 0; s < vertexCoords.size(); s++) {

        float largest_length = getLargestVertexLength(s);

        for(size_t v = 0; v < vertexCoords[s].size(); v++)
        {
            vertexCoords[s][v] /= largest_length;    
        }
    }
}

/**
 * Gets the largest length of a vector in a selected shape.
 * 
 * @param s The shape that is examined.
 * @returns largest length of vertex in the selected shape.
 */
float Loader::getLargestVertexLength(size_t s)
{
    float largest_length = 0;
    float new_length;

    for(size_t v = 0; v < vertexCoords[s].size(); v++)
    {
        // Calculate the length of the current vector.
        new_length = sqrt(pow(vertexCoords[s][v].x, 2.0) + pow(vertexCoords[s][v].y, 2.0) + pow(vertexCoords[s][v].z, 2.0));

        if(largest_length < new_length) {
            largest_length = new_length; 
        }    
    }

    return largest_length;
}

/**
 * Clears the object data in the loader object.
 */
void Loader::clearLoader()
{
    vertexCoords.clear();
    indices.clear();
    vertexNormals.clear();
    textureCoords.clear();
    colorVals.clear();
}
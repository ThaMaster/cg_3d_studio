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
 * @returns New 3D object from the file.
 */
Object Loader::parseFile(string filePath, string mFolder) 
{
    tinyobj::ObjReaderConfig readerConfig;
    readerConfig.mtl_search_path = mFolder;
    tinyobj::ObjReader reader;
    Object newObject = Object(filePath);
    if(!reader.ParseFromFile(filePath, readerConfig)) {
        // If reader detects known error.
        if (!reader.Error().empty()) {
            outputString += "\nError: \n";
            outputString += "\tTinyObjReader: ";
            outputString += reader.Error(); 
        }
        // If reader is unable to parse the file.
        return newObject;
    }

    if (!reader.Warning().empty()) {
        outputString +="\nWarning: \n";
        outputString +="\tTinyObjReader: ";
        outputString += reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    //auto& materials = reader.GetMaterials();
    
    // Allocate the number of shapes for the data vectors.
    vector<glm::vec3> vertexCoords;
    vector<unsigned int> indices;
    vector<glm::vec3> textureCoords;
    vector<glm::vec3> colorVals;
    vector<glm::vec3> vertexNormals;
    
    
    // Loop over object shapes
    for (size_t s = 0; s < shapes.size(); s++) {

        size_t index_offset = 0;
        size_t vOffset = 0;
        std::vector<unsigned char> faceVertices = shapes[s].mesh.num_face_vertices;
        newObject.oInfo.nFaces += faceVertices.size();

        // Store all vertex coordinates
        for (size_t i = 0; i < attrib.vertices.size(); i+=3) {
            vertexCoords.push_back(glm::vec3(
                attrib.vertices[i], 
                attrib.vertices[i+1], 
                attrib.vertices[i+2]));
            newObject.oInfo.nVertices++;
        }

        if(attrib.normals.size() != 0) {
            for (size_t i = 0; i < attrib.vertices.size(); i+=3) {
                vertexNormals.push_back(glm::vec3(
                    attrib.normals[i], 
                    attrib.normals[i+1], 
                    attrib.normals[i+2]));
                newObject.oInfo.nVertexNormals++;
            }
        }

        // Loop over faces(polygon)
        for (size_t f = 0; f < faceVertices.size(); f++) {
            size_t fv = size_t(faceVertices[f]);
            // Store all indices for each face
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                indices.push_back(idx.vertex_index);
                newObject.oInfo.nIndices++;
            }
            
            // Check if `normal_index` is zero or positive. negative = no normal data
            /* for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                cout << "normal_index: " << idx.normal_index << endl;
                if (idx.normal_index >= 0) {
                    vOffset = 3*size_t(idx.normal_index);
                    vertexNormals[s].push_back(glm::vec3(
                        attrib.normals[vOffset], 
                        attrib.normals[vOffset+1], 
                        attrib.normals[vOffset+2]));
                    newObject.oInfo.nNormals++;
                }
            } */

            // Check if `texcoord_index` is zero or positive. negative = no texcoord data
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                if (idx.texcoord_index >= 0) {
                    vOffset = 2*size_t(idx.texcoord_index);
                    textureCoords.push_back(glm::vec3(
                        attrib.texcoords[vOffset], 
                        attrib.texcoords[vOffset+1], 
                        attrib.texcoords[vOffset+2]));
                    newObject.oInfo.nTexCoords++;
                }
            }

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                vOffset = 3*size_t(idx.vertex_index);
                colorVals.push_back(glm::vec3(
                    attrib.colors[vOffset], 
                    attrib.colors[vOffset+1], 
                    attrib.colors[vOffset+2]));
                newObject.oInfo.nColors++;
            }

            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
        newObject.oInfo.nShapes++;
    }
    newObject.vCoords = vertexCoords;
    newObject.indices = indices;
    newObject.tCoords = textureCoords;
    newObject.colorVals = colorVals;
    newObject.vNormals = vertexNormals;
    if(newObject.oInfo.nVertexNormals == 0) newObject.produceVertexNormals();
    normalizeVertexCoords(newObject);
    newObject.oInfo.objectLoaded = true;
    return newObject;
}

/**
 * Normalizes all the shapes coordinates to fit inside the NDC cube.
 * This will change the vertexCoords value in the loader class.
 */
void Loader::normalizeVertexCoords(Object &object)
{
    float largest_length = getLargestVertexLength(object);

    for(size_t v = 0; v < object.vCoords.size(); v++)
    {
        object.vCoords[v] /= largest_length;    
    }
}

/**
 * Gets the largest length of a vector in a selected shape.
 * 
 * @returns largest length of vertex in the selected shape.
 */
float Loader::getLargestVertexLength(Object object)
{
    float largest_length = 0;
    float new_length;

    for(size_t v = 0; v < object.vCoords.size(); v++)
    {
        // Calculate the length of the current vector.
        new_length = calcVectorLength(object.vCoords[v]);

        if(largest_length < new_length) {
            largest_length = new_length; 
        }    
    }

    return largest_length;
}

float Loader::calcVectorLength(glm::vec3 v) 
{
    return sqrt(pow(v.x, 2.0) + pow(v.y, 2.0) + pow(v.z, 2.0));
}
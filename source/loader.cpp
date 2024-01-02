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
    if(!reader.ParseFromFile("./object_files/" + filePath, readerConfig)) {
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
    auto& materials = reader.GetMaterials();

    std::map<int, Object::Face> faceMap;
    
    // Allocate the number of shapes for the data vectors.
    vector<unsigned int> indices;
    vector<glm::vec3> colorVals;
    
    // Loop over object shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        size_t vOffset = 0;
        std::vector<unsigned char> faceVertices = shapes[s].mesh.num_face_vertices;
        newObject.oInfo.nFaces += faceVertices.size();
        // Store all vertex coordinates
        for (size_t i = 0; i < attrib.vertices.size(); i+=3) {
            newObject.vertices.push_back(Vertex(
                attrib.vertices[i], 
                attrib.vertices[i+1], 
                attrib.vertices[i+2]));
            newObject.oInfo.nVertices++;
        }

        if(attrib.normals.size() != 0) {
            for (size_t i = 0; i < attrib.vertices.size(); i+=3) {
                newObject.vertices[i/3].setNormal(
                    attrib.normals[i], 
                    attrib.normals[i+1], 
                    attrib.normals[i+2]);
                newObject.oInfo.nVertexNormals++;
            }
        }

        // Store all texture coordinates
        for (size_t t = 0; t < attrib.texcoords.size(); t+=2) {
            newObject.vertices[t/2].setTexCoords(
                attrib.texcoords[t], 
                attrib.texcoords[t+1]);
            newObject.oInfo.nTexCoords++;
        }

        // Loop over faces(polygon)
        for (size_t f = 0; f < faceVertices.size(); f++) {
            size_t fv = size_t(faceVertices[f]);
            int matIndex = shapes[s].mesh.material_ids[f];
            if(faceMap.find(matIndex) == faceMap.end()) {
                faceMap[matIndex] = Object::Face();
                faceMap[matIndex].materialIndex = matIndex;
                if(matIndex >= 0) {
                    newObject.oInfo.hasMaterials = true;
                    auto& mat = materials[matIndex];
                    faceMap[matIndex].mInfo.ka = glm::vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
                    faceMap[matIndex].mInfo.kd = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
                    faceMap[matIndex].mInfo.ks = glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]);
                }
            }

            // Store all indices for each face
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                indices.push_back(idx.vertex_index);
                faceMap[matIndex].indices.push_back(idx.vertex_index);
                newObject.oInfo.nIndices++;
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
        }
        
        newObject.oInfo.nShapes++;
    }

    for (auto &pair : faceMap) {
        newObject.faces.push_back(pair.second);
    }
    
    if(!newObject.oInfo.hasMaterials) newObject.oInfo.useDefaultMat = true;
    newObject.colorVals = colorVals;
    float largestVectorLength = newObject.getLargestVertexLength();
    if(newObject.oInfo.nVertexNormals == 0) newObject.produceVertexNormals();
    if(newObject.oInfo.nTexCoords == 0) newObject.produceTextureCoords(largestVectorLength);
    normalizeVertexCoords(newObject.vertices, largestVectorLength);
    newObject.oInfo.objectLoaded = true;
    return newObject;
}

/**
 * Normalizes all the shapes coordinates to fit inside the NDC cube.
 * This will change the vertexCoords value in the loader class.
 */
void Loader::normalizeVertexCoords(vector<Vertex> &vertices, float largest_length)
{
    for(Vertex &vertex : vertices)
    {
        vertex.position /= largest_length;    
    }
}
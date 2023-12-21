#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/component_wise.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

#include "vertex.h"

#define BUFFER_OFFSET(i) (reinterpret_cast<char*>(0 + (i)))

using namespace std;

class Object
{
    public:

        struct objectInfo {
            size_t nShapes = 0;
            int nVertices = 0;
            int nFaces = 0;
            int nIndices = 0;
            int nVertexNormals = 0;
            int nTexCoords = 0;
            int nColors = 0;
            bool objectLoaded = false;
            bool showWireFrame = false;
            bool showTexture = false;
            bool hasTexture = false;
            bool hasTexCoords = false;
        } oInfo;
        
        string fileName;
        vector<unsigned int> indices;
        vector<glm::vec3> vNormals;
        vector<glm::vec2> tCoords;
        vector<glm::vec3> colorVals;

        vector<Vertex> vertices;

        float matAlpha = 2.0;

        Object(string fileName)
        {
            Object::fileName = fileName;
        }

        void produceVertexNormals()
        {
            // Calculate normals per triangle and add them to each vertex normal
            vNormals.resize(vertices.size());
            for(size_t i = 0; i < indices.size(); i+=3) {
                Vertex v1 = vertices[indices[i]];
                Vertex v2 = vertices[indices[i+1]];
                Vertex v3 = vertices[indices[i+2]];

                glm::vec3 normal = glm::normalize(glm::cross(v2.position - v1.position, v3.position - v1.position));

                vNormals[indices[i]] += normal;
                vNormals[indices[i+1]] += normal;
                vNormals[indices[i+2]] += normal;
                oInfo.nVertexNormals += 3;
            }

            // Normalize each vertex normal (this effectively averages the normals)
            for(size_t n = 0; n < vNormals.size(); n++)
                vNormals[n] = glm::normalize(vNormals[n]);
        }

        vector<glm::vec3> getVertexCoords()
        {
            vector<glm::vec3> vertexCoords;
            for(Vertex vertex : vertices) vertexCoords.push_back(vertex.position);
            return vertexCoords;
        }
};

#endif
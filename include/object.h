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
        } oInfo;
        
        string fileName;
        vector<unsigned int> indices;
        vector<glm::vec3> colorVals;

        vector<Vertex> vertices;

        float matAlpha = 2.0;

        Object(string fileName)
        {
            Object::fileName = fileName;
        }

        void produceVertexNormals();
        void produceTextureCoords(float r);
        vector<glm::vec3> getVertexCoords();
        vector<glm::vec3> getVertexNormals();
        vector<glm::vec2> getTextureCoords();
        float getLargestVertexLength();

};

#endif
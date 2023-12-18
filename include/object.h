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

using namespace std;

class Object
{
    public:

        struct objectInfo {
            size_t nShapes = 0;
            int nVertices = 0;
            int nFaces = 0;
            int nIndices = 0;
            int nNormals = 0;
            int nTexCoords = 0;
            int nColors = 0;
            bool objectLoaded = false;
        } oInfo;
        
        string fileName;
        vector<vector<glm::vec3>> vCoords;
        vector<vector<unsigned int>> indices;
        vector<vector<glm::vec3>> vNormals;
        vector<vector<glm::vec3>> tCoords;
        vector<vector<glm::vec3>> colorVals;
        bool objLoaded = false;

        Object(string fileName)
        {
            Object::fileName = fileName;
        }
};

#endif
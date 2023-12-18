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
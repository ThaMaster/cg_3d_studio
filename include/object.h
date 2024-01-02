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
            bool hasMaterials = false;
            bool useDefaultMat = false;
        } oInfo;
    
        struct MaterialInfo {
            glm::vec3 ka = glm::vec3(1.0, 1.0, 1.0);
            glm::vec3 kd = glm::vec3(1.0, 1.0, 1.0);
            glm::vec3 ks = glm::vec3(1.0, 1.0, 1.0);
        };

        MaterialInfo defMat = MaterialInfo();

        struct Face {
            int materialIndex;
            MaterialInfo mInfo;
            vector<unsigned int> indices;
        };

        string fileName;

        vector<unsigned int> indices;
        vector<glm::vec3> colorVals;
        vector<Vertex> vertices;
        vector<Face> faces;

        float matAlpha = 2.0;

        GLuint vao;
        
        GLuint texture;

        // Model matrix
        glm::mat4x4 matModel = {
                            1.0, 0.0, 0.0, 0.0, 
                            0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            0.0, 0.0, 0.0, 1.0};

        Object(string fileName)
        {
            Object::fileName = fileName;
            
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // Create vertex buffer in the shared display list space and
            // bind it as VertexBuffer (GL_ARRAY_BUFFER)
            glGenBuffers( 1, &vBuffer);
            glBindBuffer( GL_ARRAY_BUFFER, vBuffer);

            /* Create buffer in the shared display list space and 
            bind it as GL_ELEMENT_ARRAY_BUFFER */
            glGenBuffers(1, &iBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);

            glBindVertexArray(0);
        }

        vector<glm::vec3> getVertexCoords();
        vector<glm::vec3> getVertexNormals();
        vector<glm::vec2> getTextureCoords();

        void sendDataToBuffers();
        void drawObject(GLuint);
        void produceVertexNormals();
        void produceTextureCoords(float r);
        float getLargestVertexLength();
        void updateModelMatrix(glm::vec3 tVals, float scVal, glm::vec3 rDir, float rotSpeed, bool &reset);
        void resetModel(bool&);

    private:
        GLuint vBuffer;
        GLuint iBuffer;
};

#endif
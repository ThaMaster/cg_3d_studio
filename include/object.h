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

        GLuint vao;
        GLuint vBuffer;
        GLuint iBuffer;

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

        void sendDataToBuffers()
        {
            glBindVertexArray(vao);
            size_t vSize = vertices.size()*sizeof(Vertex);
            size_t iSize = indices.size()*sizeof(unsigned int);
            // Position
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glEnableVertexAttribArray(0);

            // Normal
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(1);

            // Texture coordinates
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
            glEnableVertexAttribArray(2);
            
            // Allocate memory for both buffers without inserting data.
            glBufferData( GL_ARRAY_BUFFER, vSize, vertices.data(), GL_STATIC_DRAW );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, indices.data(), GL_STATIC_DRAW );
            glBindVertexArray(0);
        }

        void produceVertexNormals();
        void produceTextureCoords(float r);
        vector<glm::vec3> getVertexCoords();
        vector<glm::vec3> getVertexNormals();
        vector<glm::vec2> getTextureCoords();
        float getLargestVertexLength();

};

#endif
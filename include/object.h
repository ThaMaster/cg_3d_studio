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

/**
 * This class represents an object in this program. An object
 * contains the vertices and indices that are needed in order
 * to render the object. In this program, all faces that has
 * the same material are grouped and rendered togheter in order
 * to get good performance.
 * 
 * Each object also holds their own vertex buffer and index 
 * buffer. This is to make it possible for multiple objects to
 * be rendered in the scene simultaneously.
 * 
 * Author: Christoffer Nordlander (c20cnr@cs.umu.se)
 * 
 * Version information:
 *      2024-01-08: v1.0, first version.
 */
class Object
{
    public:

        string fileName;

        struct ObjectInfo {
            size_t nShapes = 0;
            int nVertices = 0;
            int nFaces = 0;
            int nIndices = 0;
            int nVertexNormals = 0;
            int nTexCoords = 0;
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

        float matAlpha = 2.0;
        MaterialInfo defMat = MaterialInfo();

        struct Face {
            int materialIndex;
            MaterialInfo mInfo;
            vector<unsigned int> indices;
        };

        vector<Vertex> vertices;
        vector<Face> faces;

        // Vertex buffer and texture.
        GLuint vao;
        GLuint texture;

        // Model matrix
        glm::mat4x4 matModel = {
                            1.0, 0.0, 0.0, 0.0, 
                            0.0, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0,
                            0.0, 0.0, 0.0, 1.0};

        Object(string);

        void sendDataToBuffers();
        void drawObject(GLuint);
        void produceVertexNormals();
        void produceTextureCoords(float r);
        void updateModelMatrix(glm::vec3 tVals, float scVal, glm::vec3 rDir, float rotSpeed, bool &reset);
        void resetModel(bool&);
        float getLargestVertexLength();

        vector<glm::vec3> getVertexCoords();
        vector<glm::vec3> getVertexNormals();
        vector<glm::vec2> getTextureCoords();

    private:
        GLuint vBuffer;
        GLuint iBuffer;
};

#endif
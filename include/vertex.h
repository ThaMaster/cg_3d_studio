#ifndef VERTEX_H
#define VERTEX_H

#include "glm/glm.hpp"

class Vertex
{ 
    public:
        glm::vec3 position;
        glm::vec2 texCoords;
        glm::vec3 normal;

        struct MaterialInfo {
            glm::vec3 ka;
            glm::vec3 kd;
            glm::vec3 ks;
        } mInfo;

        Vertex(float x, float y, float z)
        {
            position = glm::vec3(x,y,z);
        }

        void setMaterial(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks) 
        {
            mInfo.ka = ka;
            mInfo.kd = kd;
            mInfo.ks = ks;
        }

        void setNormal(float xn, float yn, float zn)
        {
            Vertex::normal = glm::vec3(xn, yn, zn);
        }

        void setTexCoords(float s, float t)
        {
            Vertex::texCoords = glm::vec2(s, t);
        }

        
    private:

};

#endif
#ifndef VERTEX_H
#define VERTEX_H

#include "glm/glm.hpp"

/**
 * This class represents a single vertex. A single vertex 
 * contains a position, texture coordinates and its vertex 
 * normal. It also contains functions that are regarding 
 * vertices.
 * 
 * Author: Christoffer Nordlander (c20cnr@cs.umu.se)
 * 
 * Version information:
 *      2024-01-08: v1.0, first version.
 */
class Vertex
{ 
    public:
        // Vertex attributes.
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;

        Vertex(float x, float y, float z)
        {
            Vertex::position = glm::vec3(x,y,z);
            Vertex::normal = glm::vec3(0.0, 0.0, 0.0);
            Vertex::texCoords = glm::vec2(0.0, 0.0);
        }

        void setNormal(float xn, float yn, float zn)
        {
            Vertex::normal = glm::vec3(xn, yn, zn);
        }

        void setTexCoords(float s, float t)
        {
            Vertex::texCoords = glm::vec2(s, t);
        }

        float calcVectorLength()
        {
            return sqrt(pow(position.x, 2.0) + pow(position.y, 2.0) + pow(position.z, 2.0));
        }

};

#endif
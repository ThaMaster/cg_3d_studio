#include "object.h"

void Object::produceVertexNormals()
{
    // Calculate normals per triangle and add them to each vertex normal
    for(size_t i = 0; i < indices.size(); i+=3) {
        Vertex v1 = vertices[indices[i]];
        Vertex v2 = vertices[indices[i+1]];
        Vertex v3 = vertices[indices[i+2]];

        glm::vec3 normal = glm::normalize(glm::cross(v2.position - v1.position, v3.position - v1.position));

        vertices[indices[i]].normal += normal;
        vertices[indices[i+1]].normal += normal;
        vertices[indices[i+2]].normal += normal;
        oInfo.nVertexNormals += 3;
    }

    // Normalize each vertex normal (this effectively averages the normals)
    for(size_t n = 0; n < vertices.size(); n++)
        vertices[n].normal = glm::normalize(vertices[n].normal);
}

void Object::produceTextureCoords(float r)
{
    for(Vertex &vertex : vertices)
    {
        float s = acos(vertex.position.x / r) / 3.14159265;
        float t = (atan(vertex.position.z/vertex.position.y) / 3.14159265) + 0.5;
        vertex.setTexCoords(s, t);
    }
}

vector<glm::vec3> Object::getVertexCoords()
{
    vector<glm::vec3> vertexCoords;
    for(Vertex vertex : vertices) vertexCoords.push_back(vertex.position);
    return vertexCoords;
}

vector<glm::vec3> Object::getVertexNormals()
{
    vector<glm::vec3> vertexNormals;
    for(Vertex vertex : vertices) vertexNormals.push_back(vertex.normal);
    return vertexNormals;
}

vector<glm::vec2> Object::getTextureCoords()
{
    vector<glm::vec2> textureCoords;
    for(Vertex vertex : vertices) textureCoords.push_back(vertex.texCoords);
    return textureCoords;
}

float Object::getLargestVertexLength()
{
    float largest_length = 0;
    float new_length;

    for(Vertex vertex : vertices)
    {
        // Calculate the length of the current vector.
        new_length = vertex.calcVectorLength();

        if(largest_length < new_length) {
            largest_length = new_length; 
        }    
    }

    return largest_length;
}


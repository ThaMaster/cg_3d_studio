#include "object.h"

/**
 * Function for sending the data of the object to the vertex shader. The data
 * that is sent for each vertex is:
 * 
 *      -   The Vertex position.
 *      -   The Vertex Normal.
 *      -   The Texture Coordinate.
 * 
 * After the call the objects vertex array object, array buffer and element
 * array buffer will be changed.
 */
void Object::sendDataToBuffers()
{
    glBindVertexArray(vao);
    size_t vSize = vertices.size()*sizeof(Vertex);
    size_t iSize = oInfo.nIndices*sizeof(unsigned int);

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
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, NULL, GL_STATIC_DRAW );

    int offset = 0;
    int iFaceSize; 
    for(Face face : faces) {
        iFaceSize = face.indices.size()*sizeof(unsigned int);
        glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, offset, iFaceSize, face.indices.data());
        offset += iFaceSize;
    }
    glBindVertexArray(0);
}

/**
 * Function for drawing the object to the frame buffer, using a specified shader
 * program.
 * 
 * @param program: The shader program to be used when drawing the object.
 */
void Object::drawObject(GLuint program)
{
    glBindVertexArray(vao);

    oInfo.showWireFrame? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    oInfo.showTexture? glBindTexture(GL_TEXTURE_2D, texture) : glBindTexture(GL_TEXTURE_2D, 0);

    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matModel));

    GLuint locHasTexture;
    locHasTexture = glGetUniformLocation(program, "showTexture");
    glUniform1i(locHasTexture, oInfo.showTexture);

    int offset = 0;
    for(Face face : faces) {
        if(oInfo.useDefaultMat) {
            glUniform3f(glGetUniformLocation(program, "ka"), defMat.ka.x, defMat.ka.y, defMat.ka.z);
            glUniform3f(glGetUniformLocation(program, "kd"), defMat.kd.x, defMat.kd.y, defMat.kd.z);
            glUniform3f(glGetUniformLocation(program, "ks"), defMat.ks.x, defMat.ks.y, defMat.ks.z);
        } else {
            glUniform3f(glGetUniformLocation(program, "ka"), face.mInfo.ka.x, face.mInfo.ka.y, face.mInfo.ka.z);
            glUniform3f(glGetUniformLocation(program, "kd"), face.mInfo.kd.x, face.mInfo.kd.y, face.mInfo.kd.z);
            glUniform3f(glGetUniformLocation(program, "ks"), face.mInfo.ks.x, face.mInfo.ks.y, face.mInfo.ks.z);
        }
        glDrawElements(GL_TRIANGLES, static_cast<int>(face.indices.size()), GL_UNSIGNED_INT, BUFFER_OFFSET(offset));
        offset += face.indices.size()*sizeof(unsigned int);
    }

    glBindVertexArray(0);
}

/**
 * Function for producing vertex normals for the object. The objects vertex normals
 * will be assigned after the function call. 
 * 
 * It takes the three vertices of a face and adds the face normal to each of the
 * vertex normal. After iterating all faces, normalize each of the vertex normal to
 * produce the vertex normal for a vertex of the object.
 */
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

/**
 * Function for creating texture coordinates that maps to a sphere. The
 * created coordinates will be mapped to each vertex. Should be called
 * if there are no texture already mapped to the vertices of the object.
 * 
 * @param r: The radius of the sphere to be mapped to.
 */
void Object::produceTextureCoords(float r)
{
    for(Vertex &vertex : vertices)
    {
        float s = acos(vertex.position.x / r) / 3.14159265;
        float t = (atan(vertex.position.z/vertex.position.y) / 3.14159265) + 0.5;
        vertex.setTexCoords(s, t);
        oInfo.nTexCoords++;
    }
}

/**
 * Function for getting all the vertices position of the object.
 * 
 * @return A vector with the objects vertex coordinates.
 */
vector<glm::vec3> Object::getVertexCoords()
{
    vector<glm::vec3> vertexCoords;
    for(Vertex vertex : vertices) vertexCoords.push_back(vertex.position);
    return vertexCoords;
}

/**
 * Function for getting all the vertex normals of the object.
 * 
 * @return A vector with the objects vertex normals.
 */
vector<glm::vec3> Object::getVertexNormals()
{
    vector<glm::vec3> vertexNormals;
    for(Vertex vertex : vertices) vertexNormals.push_back(vertex.normal);
    return vertexNormals;
}

/**
 * Function for getting all the texture coordinates of the object.
 * 
 * @return A vector with the objects texture coordinates.
 */
vector<glm::vec2> Object::getTextureCoords()
{
    vector<glm::vec2> textureCoords;
    for(Vertex vertex : vertices) textureCoords.push_back(vertex.texCoords);
    return textureCoords;
}

/**
 * Function for finding the largest vertex length of all the objects
 * vertices. If there are no vertices in the object, 0 will be returned.
 * 
 * @return The largest vertex length of all the objects vertices.
 */
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

/**
 * Function for updating the model matrix that affects this particular object. Will
 * not perform the operation if the value of the input is 0. The function will alter
 * the model matrix after it is called.
 * 
 * @param tVals: The translation value.
 * @param scVal: The scaling value.
 * @param rVals: The axises to rotate around.
 * @param rotSpeed: The speed of the rotation.
 * @param reset: If the model matrix should reset or not.
 */
void Object::updateModelMatrix(glm::vec3 tVals, float scVal, glm::vec3 rVals, float rotSpeed, bool &reset)
{
    // Check translation.
    if(glm::compMax(tVals) != 0 || glm::compMin(tVals) != 0)
        matModel = glm::translate(matModel, tVals);

    // Check scaling.
    if(scVal != 0)
        matModel = glm::scale(matModel, glm::vec3(scVal));

    // Check rotation.
    if(glm::compMax(rVals) != 0 || glm::compMin(rVals) != 0)
        matModel = glm::rotate(matModel, glm::radians(rotSpeed), rVals);

    // Check if object should be reset.
    resetModel(reset);
}

/**
 * Function for resetting the model matrix. If the input is true it will be
 * switch to false immediately after the operation.
 * 
 * @param reset: If the model matrix should reset or not.
 */
void Object::resetModel(bool &reset)
{
    if(reset) {
        matModel = glm::mat4(1.0f);
        reset = false;
    }
}


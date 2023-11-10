/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */

#include "geometryrender.h"
#include <glm/gtx/string_cast.hpp>

using namespace std;

// Initialize OpenGL
void GeometryRender::initialize()
{
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Create and initialize a program object with shaders
    program = initProgram("./source/shaders/vshader.glsl", "./source/shaders/fshader.glsl");
    // Installs the program object as part of current rendering state
    glUseProgram(program);

    // Creat a vertex array object
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

    // Get locations of the attributes in the shader
    locVertices = glGetAttribLocation( program, "vPosition");

    //matModel = glm::rotate(matModel, glm::radians(20.0f), glm::vec3(1,1,0));
    // matModel = glm::translate(matModel, glm::vec3(1.0f, 0.0f, 0.0f));

    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    // GLM already orders the arrays in column major, this means that we do not need to 
    // transpose the given matrix. Therefore GLboolean transpose = GL_FALSE.
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matModel));

    glBindVertexArray(0);
    glUseProgram(0);

    Loader loader;

    loadGeometry();

}

void GeometryRender::loadGeometry(void)
{
    loader.parseFile("./object_files/suzanne.obj");

    glUseProgram(program);
    glBindVertexArray(vao);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Set the pointers of locVertices to the right places
    glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(locVertices);

    // Load object data to the array buffer and index array
    size_t vSize = loader.vertexCoords[0].size()*sizeof(glm::vec3);
    size_t iSize = loader.indices[0].size()*sizeof(unsigned int);
    glBufferData( GL_ARRAY_BUFFER, vSize, loader.vertexCoords[0].data(), GL_STATIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, loader.indices[0].data(), GL_STATIC_DRAW );

    glBindVertexArray(0);
    glUseProgram(0);
}

// Check if any error has been reported from the shader
void GeometryRender::debugShader(void) const
{
    GLint  logSize;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize );
    if (logSize > 0) {
        std::cerr << "Failure in shader "  << std::endl;
        char logMsg[logSize+1];
        glGetProgramInfoLog( program, logSize, nullptr, &(logMsg[0]) );
        std::cerr << "Shader info log: " << logMsg << std::endl;
    }

}

// Render object
void GeometryRender::display()
{
    glUseProgram(program);
    glBindVertexArray(vao);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Call OpenGL to draw the triangle
    glDrawElements(GL_TRIANGLES, static_cast<int>(loader.indices[0].size()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    // Not to be called in release...
    debugShader();

    glBindVertexArray(0);
    glUseProgram(0);

}

void GeometryRender::transform(transformInfo tInfo) 
{
    glUseProgram(program);
    glBindVertexArray(vao);

    if(glm::compMax(tInfo.tVals) != 0 || glm::compMin(tInfo.tVals) != 0 ) {
        matModel = glm::translate(matModel, tInfo.tVals);
    }

    if(tInfo.scVal != 0) {
        matModel = glm::scale(matModel, glm::vec3(tInfo.scVal));
    }

    if(glm::compMax(tInfo.rVals) != 0 || glm::compMin(tInfo.rVals) != 0 ) {
        matModel = glm::rotate(matModel, glm::radians(ROT_SPEED), tInfo.rVals);
    }

    if(tInfo.reset) reset();

    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matModel));
    glBindVertexArray(0);
    glUseProgram(0);   
}

void GeometryRender::reset()
{
    glUseProgram(program);
    glBindVertexArray(vao);

    matModel = glm::mat4(1.0f);

    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matModel));
    glBindVertexArray(0);
    glUseProgram(0);  
}

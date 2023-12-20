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
    locVertices = glGetAttribLocation(program, "vPosition");
    locNormals = glGetAttribLocation(program, "vNormal");

    GLuint locCam;
    locCam = glGetUniformLocation(program, "camPos");
    glUniform3fv(locCam, 1, glm::value_ptr(wContext.cInfo.pZero));

    GLuint locLightPos;
    locLightPos = glGetUniformLocation(program, "lsPos");
    glUniform3fv(locLightPos, 1, glm::value_ptr(wContext.light.position));

    GLuint locLightColor;
    locLightColor = glGetUniformLocation(program, "lsColor");
    glUniform4fv(locLightColor, 1, glm::value_ptr(wContext.light.color));

    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.matModel));
    locModel = glGetUniformLocation( program, "V");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.matView));
    locModel = glGetUniformLocation( program, "P");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.matProj));

    glBindVertexArray(0);
    glUseProgram(0);

    Loader loader;
}

/**
 * Loads the object data to the arraybuffer and indices to the index array
 * to prepare the object to be shown.
 * 
 * @param fileName The name of the *.obj file to load the geometry from.
 */
void GeometryRender::loadGeometry(string fileName)
{
    wContext.clearObjects();
    Object newObject = loader.parseFile("./object_files/" + fileName, "./object_files/");
    // Only load the object if it successfully parsed the object file.
    if(newObject.oInfo.objectLoaded) {
        wContext.objects.push_back(newObject);
        
        glUseProgram(program);
        glBindVertexArray(vao);

        size_t vSize = wContext.getTotalVertexSize();
        size_t nSize = wContext.getTotalVertexNormalSize();
        size_t iSize = wContext.getTotalIndicesSize();
        // Set the pointers of locVertices to the right places.
        glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(locVertices);
        glVertexAttribPointer(locNormals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize));
        glEnableVertexAttribArray(locNormals);
        
        // Allocate memory for both buffers without inserting data.
        glBufferData( GL_ARRAY_BUFFER, vSize + nSize, NULL, GL_STATIC_DRAW );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, NULL, GL_STATIC_DRAW );
        
        // Fill buffers with the data.
        for(size_t o = 0; o < wContext.objects.size(); o++) {

            vSize = wContext.objects[o].vCoords.size()*sizeof(glm::vec3);
            glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, wContext.objects[o].vCoords.data());
            glBufferSubData( GL_ARRAY_BUFFER, vSize, nSize, wContext.objects[o].vNormals.data());
            
            iSize = wContext.objects[o].indices.size()*sizeof(unsigned int);
            glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, iSize, wContext.objects[o].indices.data());
        }

        glBindVertexArray(0);
        glUseProgram(0);
    }
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
    //size_t ebOffset = 0;
    if(wContext.objects.size() != 0) {
        for(size_t o = 0; o < wContext.objects.size(); o++) {
            glDrawElements(GL_TRIANGLES, static_cast<int>(wContext.objects[o].oInfo.nIndices), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
            //ebOffset = wContext.objects[o].oInfo.nIndices*sizeof(unsigned int);
        }
        // Not to be called in release...
        debugShader();
    }
    glBindVertexArray(0);
    glUseProgram(0);
}

/**
 * Updates the information regarding the object in the program. 
 * 
 * The information includes:
 *      - Translations
 *      - Scaling
 *      - Rotations
 *      - Resetting the model matrix
 *      - Load a new object in the program
 * 
 * This function runs each time a callback is detected to avoid unnecessary updates.
 * 
 */
void GeometryRender::updateObject()
{
    glUseProgram(program);

    wContext.updateMatrices();
    
    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.matModel));
    locModel = glGetUniformLocation( program, "V");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.matView));
    locModel = glGetUniformLocation( program, "P");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.matProj));
    glUseProgram(0);    
}

void GeometryRender::updateCamera()
{
    glUseProgram(program);
    GLuint locCam;
    locCam = glGetUniformLocation(program, "camPos");
    glUniform3fv(locCam, 1, glm::value_ptr(wContext.cInfo.pZero));
    glUseProgram(0);
}

void GeometryRender::updateLight()
{
    glUseProgram(program);
    GLuint locLightPos;
    locLightPos = glGetUniformLocation(program, "lsPos");
    glUniform4fv(locLightPos, 1, glm::value_ptr(wContext.light.position));

    GLuint locLightColor;
    locLightColor = glGetUniformLocation(program, "lsColor");
    glUniform4fv(locLightColor, 1, glm::value_ptr(wContext.light.color));
    glUseProgram(0);
}

void GeometryRender::updateMaterial()
{
    if(wContext.objects.size() != 0) {
        glUseProgram(program);
        GLuint locAlpha;
        locAlpha = glGetUniformLocation(program, "alpha");
        glUniform1i(locAlpha, (wContext.objects[0].matAlpha));
        glUseProgram(0);
    }
}

/**
 * Prompts the user to write the name of the object file that is to be loaded into the program.
 * The search path for the files starts in the object_files directory.
 * 
 * If no problems prasing errors are present, load the geometry of the object into the program.
 */
void GeometryRender::loadObjectFromGui(string fileName)
{
    if(!fileName.empty()) {
        loader.appendString("\nLoading ");
        loader.appendString(fileName);
        loader.appendString("...\n");
        loadGeometry(fileName);
        if(objectParseSuccess) {
            resetTransformations();
            loader.appendString("\nSuccessfully loaded \"");
            loader.appendString(fileName);
            loader.appendString("\"\n\n");
        } else {
            loader.appendString("\nFailed to load \"");
            loader.appendString(fileName);
            loader.appendString("\", returning.\n\n");
        }
    } else {
        loader.appendString("\nNo file specified, returning.\n\n");
    }
}

void GeometryRender::resetTransformations() 
{
    glUseProgram(program);

    // Reset the model matrix to the identity matrix.
    wContext.resetModel();

    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.matModel));
    glUseProgram(0); 
    
}
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
    locNormals = glGetAttribLocation(program, "vNormal");

    //matModel = glm::rotate(matModel, glm::radians(20.0f), glm::vec3(1,1,0));
    // matModel = glm::translate(matModel, glm::vec3(1.0f, 0.0f, 0.0f));

    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");

    // GLM already orders the arrays in column major, this means that we do not need to 
    // transpose the given matrix. Therefore GLboolean transpose = GL_FALSE.
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
    Object newObject = loader.parseFile("./object_files/" + fileName, "./object_files/");
    // Only load the object if it successfully parsed the object file.
    if(newObject.isValid) {
        objects.push_back(newObject);
        loader.normalizeVertexCoords(objects[0]);
        glUseProgram(program);
        glBindVertexArray(vao);

        size_t vSize = 0;
        size_t nSize = 0;
        size_t iSize = 0;
        vSize += objects[0].oInfo.nVertices*sizeof(glm::vec3);
        nSize += objects[0].oInfo.nNormals*sizeof(glm::vec3);
        iSize += objects[0].oInfo.nIndices*sizeof(unsigned int);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Set the pointers of locVertices to the right places
        glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(locVertices);
        glVertexAttribPointer(locNormals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize));
        glEnableVertexAttribArray(locNormals);
        
        // Allocate memory for both buffers without inserting data.
        glBufferData( GL_ARRAY_BUFFER, vSize + nSize, NULL, GL_STATIC_DRAW );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, 0, GL_STATIC_DRAW );
        
        // Fill buffers with the data.
        size_t vbOffset = 0;
        size_t ebOffset = 0;
        for(size_t s = 0; s < objects[0].oInfo.nShapes; s++) {
            vSize = objects[0].vCoords[s].size()*sizeof(glm::vec3);
            nSize = objects[0].vNormals[s].size()*sizeof(glm::vec3);
            iSize = objects[0].indices[s].size()*sizeof(unsigned int);
            glBufferSubData( GL_ARRAY_BUFFER, vbOffset, vSize, objects[0].vCoords[s].data());
            glBufferSubData( GL_ARRAY_BUFFER, vbOffset + vSize, nSize, objects[0].vNormals[s].data());
            glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, ebOffset, iSize, objects[0].indices[s].data());
            vbOffset += vSize + nSize;
            ebOffset += iSize;
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
    if(objects.size() != 0) {
        glDrawElements(GL_TRIANGLES, static_cast<int>(objects[0].oInfo.nIndices), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
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
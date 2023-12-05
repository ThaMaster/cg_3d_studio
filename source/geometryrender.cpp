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
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matModel));
    locModel = glGetUniformLocation( program, "V");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matView));
    locModel = glGetUniformLocation( program, "P");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matProj));

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
    loader.clearLoader();
    objectParseSuccess = loader.parseFile("./object_files/" + fileName, "./object_files/");
    // Only load the object if it successfully parsed the object file.
    if(objectParseSuccess) {
        loader.loadedFileName = fileName;
        loader.normalizeVertexCoords();
        setObjInfo();

        glUseProgram(program);
        glBindVertexArray(vao);

        size_t vSize = 0;
        size_t nSize = 0;
        size_t iSize = 0;

        for(size_t s = 0; s < loader.numberOfShapes; s++) {
            vSize += loader.vertexCoords[s].size()*sizeof(glm::vec3);
            vSize += loader.vertexNormals[s].size()*sizeof(glm::vec3);
            iSize += loader.indices[s].size()*sizeof(unsigned int);
        }

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
        for(size_t s = 0; s < loader.numberOfShapes; s++) {
            vSize = loader.vertexCoords[s].size()*sizeof(glm::vec3);
            nSize = loader.vertexNormals[s].size()*sizeof(glm::vec3);
            iSize = loader.indices[s].size()*sizeof(unsigned int);
            glBufferSubData( GL_ARRAY_BUFFER, vbOffset, vSize, loader.vertexCoords[s].data());
            glBufferSubData( GL_ARRAY_BUFFER, vbOffset + vSize, nSize, loader.vertexNormals[s].data());
            glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, ebOffset, iSize, loader.indices[s].data());
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
    
    // Call OpenGL to draw the triangle
    size_t nIndices = 0;
    for(size_t s = 0; s < loader.numberOfShapes; s++) {
        nIndices += loader.indices[s].size();
    }
    glDrawElements(GL_TRIANGLES, static_cast<int>(nIndices), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
    

    // Not to be called in release...
    debugShader();

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

    updateModelMatrix();
    updateViewMatrix();
    updateProjMatrix();
    
    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matModel));
    locModel = glGetUniformLocation( program, "V");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matView));
    locModel = glGetUniformLocation( program, "P");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matProj));
    glUseProgram(0);    
}

glm::mat4x4 GeometryRender::obliqueProjection(glm::mat4x4 m, float a, float angle)
{
    glm::mat4x4 shearMat = glm::mat4x4(1.0f);
    // Column major order, first index is the column, second index is the row.
    shearMat[2].x = a*cos(angle);
    shearMat[2].y = a*sin(angle);

    return m*shearMat;
}

/**
 * Resets the model matrix to the identity matrix. This resets the position of
 * all the transformations done to the object in the program.
 */
void GeometryRender::reset()
{
    glUseProgram(program);

    // Reset the model matrix to the identity matrix.
    matModel = glm::mat4(1.0f);
    tInfo.reset = false;

    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matModel));
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
        cout << "\nLoading " << fileName << "...\n";
        loadGeometry(fileName);
        if(objectParseSuccess) {
            reset();
            cout << "\nSuccessfully loaded \"" << fileName << "\"\n\n";
        } else {
            cout << "\nFailed to load \"" << fileName << "\", returning.\n\n";
        }
    } else {
        cout << "\nNo file specified, returning.\n\n";
    }
}

void GeometryRender::updateModelMatrix()
{
    // Check translation.
    if(glm::compMax(tInfo.tVals) != 0 || glm::compMin(tInfo.tVals) != 0)
        matModel = glm::translate(matModel, tInfo.tVals);

    // Check scaling.
    if(tInfo.scVal != 0)
        matModel = glm::scale(matModel, glm::vec3(tInfo.scVal));

    // Check rotation.
    if(glm::compMax(tInfo.rVals) != 0 || glm::compMin(tInfo.rVals) != 0)
        matModel = glm::rotate(matModel, glm::radians(ROT_SPEED), tInfo.rVals);

    // Check if object should be reset.
    if(tInfo.reset) reset();
}

void GeometryRender::updateViewMatrix()
{
    glm::vec4 newZero;
    glm::vec4 newRef;
    glm::mat4x4 rotMat = glm::mat4(1.0f);

    // Translate camera and reference in camera axises.
    if(glm::compMax(cInfo.camOffset) != 0 || glm::compMin(cInfo.camOffset) != 0) {
        newZero = matView*glm::vec4(cInfo.pZero, 1.0f);
        newRef = matView*glm::vec4(cInfo.pRef, 1.0f);
        newZero = glm::translate(glm::mat4(1.0f), cInfo.camOffset) * newZero;
        newRef = glm::translate(glm::mat4(1.0f), cInfo.camOffset) * newRef;
        newZero = glm::inverse(matView) * newZero;
        newRef = glm::inverse(matView) * newRef;
        cInfo.pZero = glm::vec3(newZero.x, newZero.y, newZero.z);
        cInfo.pRef = glm::vec3(newRef.x, newRef.y, newRef.z);
    }

    // Rotate the reference around the camera axises.
    if(cInfo.camRotOffset.x != 0 || cInfo.camRotOffset.y != 0 || cInfo.camRotOffset.z != 0) {
        if(cInfo.camRotOffset.x != 0) rotMat = glm::rotate(rotMat, glm::radians(cInfo.camRotOffset.x), glm::vec3(0,1,0));
        if(cInfo.camRotOffset.y != 0) rotMat = glm::rotate(rotMat, glm::radians(cInfo.camRotOffset.y), glm::vec3(1,0,0));
        if(cInfo.camRotOffset.z != 0) rotMat = glm::rotate(rotMat, glm::radians(cInfo.camRotOffset.z), glm::vec3(0,0,1));
        // Does not need to update the camera direction, the viewModel resets the camera position when altered.
        newRef = glm::inverse(matView) * (rotMat * glm::vec4(cInfo.camDir, 1.0f));
        cInfo.pRef = glm::vec3(newRef.x, newRef.y, newRef.z);
    }
    matView = glm::lookAt(cInfo.pZero, cInfo.pRef, cInfo.upVec);
}

void GeometryRender::updateProjMatrix()
{
    if(cInfo.perspProj) {
        matProj = glm::perspective(glm::radians(cInfo.fov), getAspectRatio(), cInfo.nearPlane, cInfo.farPlane);
    } else {
        float aspect = width() / height();
        matProj = glm::ortho(-cInfo.top*aspect, cInfo.top*aspect, -cInfo.top, cInfo.top, cInfo.nearPlane, cInfo.farPlane);
        if(cInfo.obliqueScale != 0.0f)
            matProj = obliqueProjection(matProj, cInfo.obliqueScale, cInfo.obliqueAngleRad);
    }
}

void GeometryRender::setObjInfo()
{
    oInfo.objectLoaded = true;
    oInfo.nShapes = loader.numberOfShapes;
    oInfo.nVertices = loader.numberOfVertices;
    oInfo.nFaces = loader.numberOfFaces;
    oInfo.nIndices = loader.numberOfIndices;
    oInfo.nNormals = loader.numberOfNormals;
    oInfo.nTexCoords = loader.numberOfTexCoords;
    oInfo.nColors = loader.numberOfColors;
}

void GeometryRender::clearObjInfo()
{
    oInfo.objectLoaded = false;
    oInfo.nShapes = 0;
    oInfo.nVertices = 0;
    oInfo.nFaces = 0;
    oInfo.nIndices = 0;
    oInfo.nNormals = 0;
    oInfo.nTexCoords = 0;
    oInfo.nColors = 0;
}
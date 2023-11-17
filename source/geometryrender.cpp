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
    objectParseSuccess = loader.parseFile("./object_files/" + fileName);
    // Only load the object if it successfully parsed the object file.
    if(objectParseSuccess) {
        loader.loadedFileName = fileName;
        loader.normalizeVertexCoords();

        glUseProgram(program);
        glBindVertexArray(vao);
        
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        // Set the pointers of locVertices to the right places
        glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
        glEnableVertexAttribArray(locVertices);

        size_t vSize = 0;
        size_t iSize = 0;
        
        // Calculate the total size needed for vertices and indices.
        for(size_t s = 0; s < loader.numberOfShapes; s++) {
            vSize += loader.vertexCoords[s].size()*sizeof(glm::vec3);
            iSize += loader.indices[s].size()*sizeof(unsigned int);
        }
        
        // Allocate memory for both buffers without inserting data.
        glBufferData( GL_ARRAY_BUFFER, vSize, 0, GL_STATIC_DRAW );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, 0, GL_STATIC_DRAW );

        // Fill both buffers with the data.
        size_t vbOffset = 0;
        size_t ebOffset = 0;
        for(size_t s = 0; s < loader.numberOfShapes; s++) {
            vSize = loader.vertexCoords[s].size()*sizeof(glm::vec3);
            iSize = loader.indices[s].size()*sizeof(unsigned int);
            glBufferSubData( GL_ARRAY_BUFFER, vbOffset, vSize, loader.vertexCoords[s].data());
            glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, ebOffset, iSize, loader.indices[s].data());
            vbOffset += vSize;
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
 * @param oInfo Struct containing the different flags and values to 
 *              update the object in the program.
 */
void GeometryRender::updateObject(objectInfo oInfo) 
{
    glUseProgram(program);
    glBindVertexArray(vao);

    // Check translation.
    if(glm::compMax(oInfo.tVals) != 0 || glm::compMin(oInfo.tVals) != 0 )
        matModel = glm::translate(matModel, oInfo.tVals);

    // Check scaling.
    if(oInfo.scVal != 0)
        matModel = glm::scale(matModel, glm::vec3(oInfo.scVal));

    // Check rotation.
    if(glm::compMax(oInfo.rVals) != 0 || glm::compMin(oInfo.rVals) != 0 )
        matModel = glm::rotate(matModel, glm::radians(ROT_SPEED), oInfo.rVals);

    // Check if object should be reset.
    if(oInfo.reset) reset();

    // Check if new object should be loaded.
    if(oInfo.loadObject) loadObjectFromTerminal();

    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matModel));
    glBindVertexArray(0);
    glUseProgram(0);   
}

/**
 * Resets the model matrix to the identity matrix. This resets the position of
 * all the transformations done to the object in the program.
 */
void GeometryRender::reset()
{
    glUseProgram(program);
    glBindVertexArray(vao);

    // Reset the model matrix to the identity matrix.
    matModel = glm::mat4(1.0f);

    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(matModel));
    glBindVertexArray(0);
    glUseProgram(0);  
}

/**
 * Prompts the user to write the name of the object file that is to be loaded into the program.
 * The search path for the files starts in the object_files directory.
 * 
 * The function handles if the input string is empty and gives correct feedback if other 
 * errors have occurd. If there are any warnings these will be displayed but the object will
 * still load into the program.
 * 
 * If no problems prasing errors are present, load the geometry of the object into the program.
 */
void GeometryRender::loadObjectFromTerminal() 
{
    string fileName;
    cout << "Write the name of the file: ";
    getline(cin, fileName);
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

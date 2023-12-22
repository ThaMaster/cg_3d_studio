/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#include "geometryrender.h"
#include <glm/gtx/string_cast.hpp>
#include "stb_image.h"

using namespace std;

// Initialize OpenGL
void GeometryRender::initialize()
{
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Create and initialize a program object with shaders
    program1 = initProgram("./source/shaders/vshader.glsl", "./source/shaders/fshader.glsl");

    // Installs the program object as part of current rendering state
    glUseProgram(program1);
    setupShaderProgram(program1);

    // Creat a vertex array object
    glBindVertexArray(vao);

    // Create vertex buffer in the shared display list space and
    // bind it as VertexBuffer (GL_ARRAY_BUFFER)
    glGenBuffers( 1, &vBuffer);
    glBindBuffer( GL_ARRAY_BUFFER, vBuffer);

    /* Create buffer in the shared display list space and 
       bind it as GL_ELEMENT_ARRAY_BUFFER */
    glGenBuffers(1, &iBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
    
    glBindVertexArray(0);
    glUseProgram(0);

    Loader loader;
}

GeometryRender::~GeometryRender()
{
    glDeleteBuffers(1, &vBuffer);
    glDeleteBuffers(1, &iBuffer);
    glDeleteVertexArrays(1, &vao);
    // Add deletion of other resources as needed
}

void GeometryRender::setupShaderProgram(GLuint program) 
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    // Get locations of the attributes in the shader
    locVertices = glGetAttribLocation(program, "vPosition");
    locNormals = glGetAttribLocation(program, "vNormal");
    locTextures = glGetAttribLocation(program, "aTexCoord");

    GLuint locAmbi;
    locAmbi = glGetUniformLocation(program, "la");
    glUniform4fv(locAmbi, 1, glm::value_ptr(wContext.ambientLight));

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
        glUseProgram(program1);
        glBindVertexArray(vao);
        for(Object object : wContext.objects) {

            size_t vSize = object.vertices.size()*sizeof(glm::vec3);
            size_t nSize = object.getVertexNormals().size()*sizeof(glm::vec3);
            size_t iSize = object.indices.size()*sizeof(unsigned int);
            size_t tSize = object.getTextureCoords().size()*sizeof(glm::vec2);

            // Set the pointers of locVertices to the right places.
            glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(locVertices);
            glVertexAttribPointer(locNormals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize));
            glEnableVertexAttribArray(locNormals);
            glVertexAttribPointer(locTextures, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize+nSize));
            glEnableVertexAttribArray(locTextures);
            
            // Allocate memory for both buffers without inserting data.
            glBufferData( GL_ARRAY_BUFFER, vSize + nSize + tSize, NULL, GL_STATIC_DRAW );
            glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, NULL, GL_STATIC_DRAW );
            
            // Fill buffers with the data.
            glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, object.getVertexCoords().data());
            glBufferSubData( GL_ARRAY_BUFFER, vSize, nSize, object.getVertexNormals().data());
            glBufferSubData( GL_ARRAY_BUFFER, vSize+nSize, tSize, object.getTextureCoords().data());
            glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, iSize, object.indices.data());
        }
        glBindVertexArray(0);
        glUseProgram(0);
    }
}

// Check if any error has been reported from the shader
void GeometryRender::debugShader(void) const
{
    GLint  logSize;
    glGetProgramiv( program1, GL_INFO_LOG_LENGTH, &logSize );
    if (logSize > 0) {
        std::cerr << "Failure in shader "  << std::endl;
        char logMsg[logSize+1];
        glGetProgramInfoLog( program1, logSize, nullptr, &(logMsg[0]) );
        std::cerr << "Shader info log: " << logMsg << std::endl;
    }
}

// Render object
void GeometryRender::display()
{
    glUseProgram(program1);
    glBindVertexArray(vao);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //size_t ebOffset = 0;
    if(wContext.objects.size() != 0) {
        for(size_t o = 0; o < wContext.objects.size(); o++) {
            wContext.objects[o].oInfo.showWireFrame? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            wContext.objects[o].oInfo.showTexture? glBindTexture(GL_TEXTURE_2D, texture) : glBindTexture(GL_TEXTURE_2D, 0);
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
    glUseProgram(program1);

    wContext.updateMatrices();
    
    GLuint locModel;
    locModel = glGetUniformLocation( program1, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.matModel));
    locModel = glGetUniformLocation( program1, "V");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.matView));
    locModel = glGetUniformLocation( program1, "P");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.matProj));
    glUseProgram(0);    
}

void GeometryRender::updateCamera()
{
    glUseProgram(program1);
    GLuint locCam;
    locCam = glGetUniformLocation(program1, "camPos");
    glUniform3fv(locCam, 1, glm::value_ptr(wContext.cInfo.pZero));
    glUseProgram(0);
}

void GeometryRender::updateLight()
{
    glUseProgram(program1);
    GLuint locLightPos;
    locLightPos = glGetUniformLocation(program1, "lsPos");
    glUniform4fv(locLightPos, 1, glm::value_ptr(wContext.light.position));

    GLuint locLightColor;
    locLightColor = glGetUniformLocation(program1, "lsColor");
    glUniform4fv(locLightColor, 1, glm::value_ptr(wContext.light.color));

    // THIS IS AMBIENT LIGHT AND SHOULD MAYBE CHANGE POSITION LATER!
    GLuint locAmbi;
    locAmbi = glGetUniformLocation(program1, "la");
    glUniform4fv(locAmbi, 1, glm::value_ptr(wContext.ambientLight));
    glUseProgram(0);
}

void GeometryRender::updateMaterial()
{
    if(wContext.objects.size() != 0) {
        glUseProgram(program1);
        GLuint locAlpha;
        locAlpha = glGetUniformLocation(program1, "alpha");
        glUniform1f(locAlpha, (wContext.objects[0].matAlpha));
        GLuint locHasTexture;
        locHasTexture = glGetUniformLocation(program1, "hasTexture");
        glUniform1i(locHasTexture, wContext.objects[0].oInfo.showTexture);
        glUseProgram(0);
    }
}

/**
 * Prompts the user to write the name of the object file that is to be loaded into the program.
 * The search path for the files starts in the object_files directory.
 * 
 * If no problems prasing errors are present, load the geometry of the object into the program.
 */
string GeometryRender::loadObjectFromGui(string objName)
{
    if(!objName.empty()) {
        loader.outputString += "\nLoading ";
        loader.outputString += objName;
        loader.outputString += "...\n";
        loadGeometry(objName);
        if(objectParseSuccess) {
            resetTransformations();
            loader.outputString += "\nSuccessfully loaded \"";
            loader.outputString += objName;
            loader.outputString += "\"\n\n";
        } else {
            loader.outputString += "\nFailed to load \"";
            loader.outputString += objName;
            loader.outputString += "\", returning.\n\n";
        }
    } else {
        loader.outputString += "\nNo file specified, returning.\n\n";
    }
    return loader.getOutputString();
}

string GeometryRender::loadTextureFromGui(string textureName)
{
    string outputString = "";
    if(!textureName.empty()) {
        outputString += "\nLoading ";
        outputString += textureName;
        outputString += "...\n";
        outputString += loadTexture(textureName);
    } else {
        outputString += "\nNo texture specified, returning.\n\n";
    }
    return outputString;
}

void GeometryRender::resetTransformations() 
{
    glUseProgram(program1);

    // Reset the model matrix to the identity matrix.
    wContext.resetModel();

    GLuint locModel;
    locModel = glGetUniformLocation( program1, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.matModel));
    glUseProgram(0); 
}

string GeometryRender::loadTexture(string textureName)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(("./textures/" + textureName).c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        return "\nFailed to load texture \"" + textureName + "\"\n";
    }
    stbi_image_free(data);
    wContext.objects[0].oInfo.hasTexture = true;
    wContext.objects[0].oInfo.showTexture = true;
    return "\nSuccessfully loaded texture \"" + textureName + "\"\n";
}
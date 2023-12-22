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
    program = initProgram("./source/shaders/vshader.glsl", "./source/shaders/fshader.glsl");

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
    Object newObject = loader.parseFile(fileName, "./object_files/");
    
    // Only load the object if it successfully parsed the object file.
    if(newObject.oInfo.objectLoaded) {
        glUseProgram(program);
        newObject.sendDataToBuffers();
        glUseProgram(0);
        wContext.objects.push_back(newObject);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(Object &object : wContext.objects) {
        glBindVertexArray(object.vao);

        object.oInfo.showWireFrame? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        object.oInfo.showTexture? glBindTexture(GL_TEXTURE_2D, object.texture) : glBindTexture(GL_TEXTURE_2D, 0);

        GLuint locModel;
        locModel = glGetUniformLocation( program, "M");
        glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(object.matModel));
        glDrawElements(GL_TRIANGLES, static_cast<int>(object.oInfo.nIndices), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

        glBindVertexArray(0);
    }
    // Not to be called in release...
    debugShader();
    
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
void GeometryRender::updateObject(int objIndex)
{
    glUseProgram(program);

    wContext.updateMatrices();
    
    GLuint locView;
    locView = glGetUniformLocation( program, "V");
    glUniformMatrix4fv(locView, 1, GL_FALSE, glm::value_ptr(wContext.matView));
    GLuint locProj;
    locProj = glGetUniformLocation( program, "P");
    glUniformMatrix4fv(locProj, 1, GL_FALSE, glm::value_ptr(wContext.matProj));
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

    // THIS IS AMBIENT LIGHT AND SHOULD MAYBE CHANGE POSITION LATER!
    GLuint locAmbi;
    locAmbi = glGetUniformLocation(program, "la");
    glUniform4fv(locAmbi, 1, glm::value_ptr(wContext.ambientLight));
    glUseProgram(0);
}

void GeometryRender::updateMaterial(int objIndex)
{
    if(wContext.objects.size() != 0) {
        glUseProgram(program);
        GLuint locAlpha;
        locAlpha = glGetUniformLocation(program, "alpha");
        glUniform1f(locAlpha, (wContext.objects[objIndex].matAlpha));
        GLuint locHasTexture;
        locHasTexture = glGetUniformLocation(program, "hasTexture");
        glUniform1i(locHasTexture, wContext.objects[objIndex].oInfo.hasTexture);
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

string GeometryRender::loadTextureFromGui(string textureName, int objIndex)
{
    string outputString = "";
    if(!textureName.empty()) {
        outputString += "\nLoading ";
        outputString += textureName;
        outputString += "...\n";
        outputString += loadTexture(textureName, wContext.objects[objIndex].texture, objIndex);
    } else {
        outputString += "\nNo texture specified, returning.\n\n";
    }
    return outputString;
}

void GeometryRender::resetTransformations(int objIndex) 
{
    glUseProgram(program);

    // Reset the model matrix to the identity matrix.
    wContext.objects[objIndex].resetModel(wContext.tInfo.reset);

    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.objects[objIndex].matModel));
    glUseProgram(0); 
}

string GeometryRender::loadTexture(string textureName, GLuint &texture, int selectedObject)
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
    wContext.objects[selectedObject].oInfo.hasTexture = true;
    wContext.objects[selectedObject].oInfo.showTexture = true;
    return "\nSuccessfully loaded texture \"" + textureName + "\"\n";
}
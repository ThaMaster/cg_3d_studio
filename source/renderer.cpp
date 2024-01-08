#include "renderer.h"

using namespace std;

/**
 * This class renders the objects and handles the OpenGL shader files such 
 * as loading the files, loading objects, loading textures and more.
 * It inherits the Studio3D object and overwrites certain functions in 
 * order to make them available in the main program.
 * 
 * Author: Christoffer Nordlander (c20cnr@cs.umu.se)
 * 
 * Version information:
 *      2024-01-08: v1.0, first version.
 */

/**
 * Initialize the renderer with depth test and
 * loads the two shader files.
 */ 
void Renderer::initialize()
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
void Renderer::loadGeometry(string fileName)
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

/**
 * Function for checking if any error has been reported from 
 * the shader.
 */
void Renderer::debugShader(void) const
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

/**
 * Function for rendering all the loaded objects in the
 * scene. If no objects has been loaded nohting will 
 * happen.
 */
void Renderer::display()
{
    glUseProgram(program);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for(Object &object : wContext.objects) {
        object.drawObject(program);
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
 * The object that where these updates occurs are the selected
 * object only.
 * 
 * @param objIndex: The index of the selected object.
 */
void Renderer::updateObject(int objIndex)
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

/**
 * Function for updating the camera values
 * in the shader files.
 */
void Renderer::updateCamera()
{
    glUseProgram(program);
    GLuint locCam;
    locCam = glGetUniformLocation(program, "camPos");
    glUniform3fv(locCam, 1, glm::value_ptr(wContext.cInfo.pZero));
    glUseProgram(0);
}

/**
 * Functoin for updating the values of the
 * light in the shader files.
 */
void Renderer::updateLight()
{
    glUseProgram(program);
    
    GLuint locAmbi;
    locAmbi = glGetUniformLocation(program, "la");
    glUniform4fv(locAmbi, 1, glm::value_ptr(wContext.ambientLight));

    GLuint locLightPos;
    locLightPos = glGetUniformLocation(program, "lsPos");
    glUniform4fv(locLightPos, 1, glm::value_ptr(wContext.light.position));

    GLuint locLightColor;
    locLightColor = glGetUniformLocation(program, "lsColor");
    glUniform4fv(locLightColor, 1, glm::value_ptr(wContext.light.color));
    glUseProgram(0);
}

/**
 * Function for updating the material on the selected object.
 * 
 * @param objIndex: The index of the selected object.
 */
void Renderer::updateMaterial(int objIndex)
{
    if(wContext.objects.size() != 0) {
        glUseProgram(program);
        GLuint locAlpha;
        locAlpha = glGetUniformLocation(program, "alpha");
        glUniform1f(locAlpha, (wContext.objects[objIndex].matAlpha));
        glUseProgram(0);
    }
}

/**
 * Loads the object with the specified object file name.
 * If any errors are occurs these will be added to the 
 * output of the function and later displayed in the 
 * logger.
 * 
 * @param objName: The name of the object file.
 * 
 * @return The output string.
 */
string Renderer::loadObjectFromGui(string objName)
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

/**
 * Attempts to load the specified texture for the currently
 * selected object. If any errors occurs when trying to
 * load the texture it will be displayed in the output string.
 * 
 * @param textureName: The name of the texture file.
 * @param objIndex: The index of the selected object.
 * 
 * @return The output string.
 */
string Renderer::loadTextureFromGui(string textureName, int objIndex)
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

/**
 * Function for resetting the model matrix for the
 * currently selected object. It will restore the
 * objects model matrix to the identity matrix.
 * 
 * @param objIndex: The index of the selected object.
 */
void Renderer::resetTransformations(int objIndex) 
{
    glUseProgram(program);

    // Reset the model matrix to the identity matrix.
    wContext.objects[objIndex].resetModel(wContext.tInfo.reset);

    GLuint locModel;
    locModel = glGetUniformLocation( program, "M");
    glUniformMatrix4fv(locModel, 1, GL_FALSE, glm::value_ptr(wContext.objects[objIndex].matModel));
    glUseProgram(0); 
}

/**
 * Function for loading a texture for the currently
 * selected object. It will overwrite the texture value
 * of the selected object which will cause the texture
 * to change.
 * 
 * @param textureName: The name of the texture file.
 * @param texture: The actual texture values for the selected object.
 * @param selectedObject: The index of the selected object.
 * 
 * @return The output string.
 */
string Renderer::loadTexture(string textureName, GLuint &texture, int selectedObject)
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
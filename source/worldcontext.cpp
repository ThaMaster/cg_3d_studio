#include "worldcontext.h"

/**
 * The world context class is to represent all the information
 * of the rendered scene. It will contain all the loaded objects,
 * camera position, lights and more. This class is also the
 * object that holds all the transformation values and camera
 * values that are used to transform objects and move the camera.
 * 
 * Since most of the projects components are in need for the
 * information stored in this class, the world context class
 * is very hard coupled.
 * 
 * Author: Christoffer Nordlander (c20cnr@cs.umu.se)
 * 
 * Version information:
 *      2024-01-08: v1.0, first version.
 */

/**
 * Fuinction for getting the aspect ratio of the 
 * window.
 * 
 * @return Float representing the aspect ration.
 */
float WorldContext::getAspectRatio() 
{
    return (float)windowWidth/(float)windowHeight;
}

/**
 * Function for updating the matrices that is used
 * to produce the scene. Only the selected objects
 * model matrix will be updated when transformations
 * are occurring.
 */
void WorldContext::updateMatrices() 
{
    objects[selectedObject].updateModelMatrix(tInfo.tVals, tInfo.scVal, tInfo.rVals, ROT_SPEED, tInfo.reset);
    updateViewMatrix();
    updateProjMatrix();
}

/**
 * Function for updating the view matrix of the
 * scene. It will however only update the values
 * if any changes to the offsets are made.
 */
void WorldContext::updateViewMatrix()
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

/**
 * Function for updating the projection matrix of
 * the scene. It will update the matrix in differnet
 * ways depending on which projection to use.
 */
void WorldContext::updateProjMatrix()
{
    if(cInfo.perspProj) {
        matProj = glm::perspective(glm::radians(cInfo.fov), getAspectRatio(), cInfo.nearPlane, cInfo.farPlane);
    } else {
        matProj = glm::ortho(-cInfo.top, cInfo.top, -cInfo.top, cInfo.top, cInfo.nearPlane, cInfo.farPlane);
        if(cInfo.obliqueScale != 0.0f)
            matProj = obliqueProjection(matProj, cInfo.obliqueScale, cInfo.obliqueAngleRad);
    }
}

/**
 * Function for applying oblique projection and returning
 * the resulting matrix.
 * 
 * @param m: The matrix to be altered.
 * @param a: The scaling factor to be applied.
 * @param angle: The angle of the oblique projection.
 * 
 * @return The oblique projection matrix.
 */
glm::mat4x4 WorldContext::obliqueProjection(glm::mat4x4 m, float a, float angle)
{
    glm::mat4x4 shearMat = glm::mat4x4(1.0f);
    // Column major order, first index is the column, second index is the row.
    shearMat[2].x = a*cos(angle);
    shearMat[2].y = a*sin(angle);

    return m*shearMat;
}

/**
 * Function for clearing all the loaded objects
 * in the scene.
 */
void WorldContext::clearObjects()
{
    objects.clear();
    selectedObject = 0;
}
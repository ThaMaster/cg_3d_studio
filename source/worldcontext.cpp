#include "worldcontext.h"

float WorldContext::getAspectRatio() 
{
    return (float)windowWidth/(float)windowHeight;
}

void WorldContext::updateMatrices() 
{
    updateModelMatrix();
    updateViewMatrix();
    updateProjMatrix();
}

void WorldContext::updateModelMatrix()
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
    if(tInfo.reset) resetModel();
}

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

glm::mat4x4 WorldContext::obliqueProjection(glm::mat4x4 m, float a, float angle)
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
void WorldContext::resetModel()
{
    // Reset the model matrix to the identity matrix.
    matModel = glm::mat4(1.0f);
    tInfo.reset = false;
}

void WorldContext::clearObjects()
{
    objects.clear();
}

size_t WorldContext::getTotalVertexSize() 
{
    size_t totalVertexSize = 0;
    for(size_t o = 0; o < objects.size(); o++)
    {
        totalVertexSize += objects[o].oInfo.nVertices*sizeof(glm::vec3);
    }
    return totalVertexSize;
}

size_t WorldContext::getTotalVertexNormalSize() 
{
    size_t totalVertexNormalSize = 0;
    for(size_t o = 0; o < objects.size(); o++)
    {
        totalVertexNormalSize += objects[o].oInfo.nVertexNormals*sizeof(glm::vec3);
    }
    return totalVertexNormalSize;
}

size_t WorldContext::getTotalIndicesSize() 
{
    size_t totalIndicesSize = 0;
    for(size_t o = 0; o < objects.size(); o++)
    {
        totalIndicesSize += objects[o].oInfo.nIndices*sizeof(unsigned int);
    }
    return totalIndicesSize;
}

size_t WorldContext::getTotalTextureSize() 
{
    size_t totalTextureSize = 0;
    for(size_t o = 0; o < objects.size(); o++)
    {
        totalTextureSize += objects[o].oInfo.nTexCoords*sizeof(glm::vec2);
    }
    return totalTextureSize;
}
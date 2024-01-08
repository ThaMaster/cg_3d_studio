#ifndef WORLDCONTEXT_H
#define WORLDCONTEXT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/component_wise.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "loader.h"
#include "lightsource.h"

const float pi_f = 3.1415926f;
const int windowWidth = 1024;
const int windowHeight = 768;

class WorldContext
{
    public:

        float ROT_SPEED = 5.0f;
        float TRA_SPEED = 0.1f;
        float SCA_SPEED = 0.1f;
        float CAM_SPEED = 0.1f;

        struct transformInfo {
            glm::vec3 rVals = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 tVals = glm::vec3(0.0f, 0.0f, 0.0f);
            float scVal = 0.0f;
            bool reset = false;
        } tInfo;
        
        struct cameraInfo {
            glm::vec3 pZero = glm::vec3(0.0f, 0.0f, 2.0f);
            glm::vec3 pRef = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 camDir = glm::vec3(pRef- pZero);
            glm::vec3 upVec = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::vec3 camOffset = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 camRotOffset = glm::vec3(0.0f, 0.0f, 0.0f);
            float fov = 60.0f;
            float farPlane = 500.0f;
            float nearPlane = 0.1f;
            float top = 1.0f;
            float obliqueScale = 0.0f;
            float obliqueAngleRad = pi_f/4.0f;
            bool perspProj = true;
        } cInfo;

        int selectedObject = 0;

        const glm::vec3 pZeroDefault = glm::vec3(0.0f, 0.0f, 2.0f);
        const glm::vec3 pRefDefault = glm::vec3(0.0f, 0.0f, 0.0f);

        vector<Object> objects;
        vector<LightSource> lights;

        LightSource light = LightSource(glm::vec4( 0.0, 5.0, 0.0, 0.0), glm::vec4( 1.0, 1.0, 1.0, 1.0 ));
        glm::vec4 ambientLight = glm::vec4(0.1, 0.1, 0.1, 1.0);
        const glm::vec4 defaultAmbientLight = glm::vec4(0.1, 0.1, 0.1, 1.0);

        // View matrix
        glm::mat4x4 matView = glm::lookAt(cInfo.pZero, cInfo.pRef, cInfo.upVec);
        
        // Projection matrix
        glm::mat4x4 matProj = glm::perspective(glm::radians(cInfo.fov), getAspectRatio(), cInfo.nearPlane, cInfo.farPlane);

        void updateMatrices();
        void clearObjects();
    private:

        glm::mat4x4 obliqueProjection(glm::mat4x4, float, float);

        void updateViewMatrix();
        void updateProjMatrix();

        float getAspectRatio();
};

#endif
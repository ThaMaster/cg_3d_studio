/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include "openglwindow.h"
#include "loader.h"

class GeometryRender : public OpenGLWindow
{
public:
    template<typename... ARGS>
    GeometryRender(ARGS&&... args) : OpenGLWindow{ std::forward<ARGS>(args)... }
    {}

    void initialize();
    virtual void display() override;
    void updateObject() override;
    void reset() override;
    void loadObjectFromGui(string) override;
    void loadObjectFromTerminal();

private:
    GLuint program;

    Loader loader;
    
    // OpenGL buffers
    GLuint vao;
    GLuint vBuffer;
    GLuint iBuffer;

    // OpenGL attribute locations
    GLuint locVertices;

    // Model matrix
    glm::mat4x4 matModel = {
                        1.0, 0.0, 0.0, 0.0, 
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0};
    // View matrix
    glm::mat4x4 matView = glm::lookAt(cInfo.pZero, cInfo.pRef, cInfo.upVec);
    
    // Projection matrix
    glm::mat4x4 matProj = glm::perspective(glm::radians(cInfo.fov), getAspectRatio(), cInfo.nearPlane, cInfo.farPlane);

    bool objectParseSuccess;

    void debugShader(void) const;

    void loadGeometry(string fileName);
    glm::mat4x4 obliqueProjection(glm::mat4x4, float, float);
    glm::mat3x3 getFinalRotMat();
    void updateModelMatrix();
    void updateViewMatrix();
    void updateProjMatrix();
    void setObjInfo();
    void clearObjInfo();
};

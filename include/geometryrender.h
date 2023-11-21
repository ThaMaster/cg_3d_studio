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
    void updateObject(objectInfo, cameraInfo) override;
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
    glm::mat4x4 matView = {
                        1.0, 0.0, 0.0, 0.0, 
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0};
    // Projection matrix
    glm::mat4x4 matProj = {
                        1.0, 0.0, 0.0, 0.0, 
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0};

    bool objectParseSuccess;

    void debugShader(void) const;

    void loadGeometry(string fileName);
    float calcVectorLength(glm::vec3);
    glm::mat4x4 obliqueProjection(glm::mat4x4, float, float);
};

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
    void transform(transformInfo) override;
    void reset() override;
    void loadObject();
    bool objectLoadError = false;
    bool viewLines = false;

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
    glm::mat4x4 matModel = {1.0, 0.0, 0.0, 0.0, 
                        0.0, 1.0, 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0};

    void debugShader(void) const;

    void loadGeometry(string fileName);
};

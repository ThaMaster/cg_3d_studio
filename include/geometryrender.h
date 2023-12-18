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
    vector<Object> objects;
    template<typename... ARGS>
    GeometryRender(ARGS&&... args) : OpenGLWindow{ std::forward<ARGS>(args)... }
    {}

    void initialize();
    virtual void display() override;
    void updateObject() override;
    void loadObjectFromGui(string) override;

private:
    GLuint program;

    Loader loader;
    
    // OpenGL buffers
    GLuint vao;
    GLuint vBuffer;
    GLuint iBuffer;

    // OpenGL attribute locations
    GLuint locVertices;
    GLuint locNormals;

    bool objectParseSuccess;

    void debugShader(void) const;

    void loadGeometry(string fileName);

    void resetTransformations();
};

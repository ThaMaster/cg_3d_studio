#pragma once

#include "openglwindow.h"
#include "loader.h"
#include "stb_image.h"

class GeometryRender : public OpenGLWindow
{
public:
    template<typename... ARGS>
    GeometryRender(ARGS&&... args) : OpenGLWindow{ std::forward<ARGS>(args)... }
    {}
    
    void initialize();
    virtual void display() override;
    void updateObject(int) override;
    void updateCamera() override;
    void updateLight() override;
    void updateMaterial(int) override;
    string loadObjectFromGui(string) override;
    string loadTextureFromGui(string, int) override;

private:
    GLuint program;
    Loader loader;
    bool objectParseSuccess;

    void debugShader(void) const;
    void loadGeometry(string);
    void resetTransformations(int);
    string loadTexture(string, GLuint&, int);
};
